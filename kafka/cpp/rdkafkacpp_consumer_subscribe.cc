#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

static bool run = true;
static void stop(int sig)
{
    run = false;
}

static void print_time ()
{
    struct timeval tv;
    char buf[64];
    gettimeofday(&tv, NULL);
    strftime(buf, sizeof(buf) - 1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
    fprintf(stderr, "%s.%03d: ", buf, (int)(tv.tv_usec / 1000));
}

class ExampleEventCb : public RdKafka::EventCb
{
public:
    void event_cb(RdKafka::Event & event) override
    {
        print_time();
        switch (event.type()) {
            case RdKafka::Event::EVENT_ERROR:
                std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): "
                    << event.str() << std::endl;
                if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN) {
                    run = false;
                }
                break;

            case RdKafka::Event::EVENT_STATS:
                std::cerr << "\"STATS\": " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_LOG:
                fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(), event.str().c_str());
                break;

            case RdKafka::Event::EVENT_THROTTLE:
                std::cerr << "THROTTLED: " << event.throttle_time() << "ms by "
                    << event.broker_name() << " id " << (int)event.broker_id()
                    << std::endl;
                    break;

            default:
                std::cerr << "EVENT " << event.type()
                    << " (" << RdKafka::err2str(event.err()) << "): "
                    << event.str() << std::endl;
                break;
        }
    }
};

void msg_consume(RdKafka::Message *message, void *opaque)
{
    switch (message->err()) {
        case RdKafka::ERR__TIMED_OUT:
            break;

        case RdKafka::ERR_NO_ERROR:
            /* Real message */
            RdKafka::MessageTimestamp ts;
            ts = message->timestamp();
            if (ts.type != RdKafka::MessageTimestamp::MSG_TIMESTAMP_NOT_AVAILABLE) {
                std::string tsname = "?";
                if (ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_CREATE_TIME)
                    tsname = "create time";
                else if (ts.type == RdKafka::MessageTimestamp::MSG_TIMESTAMP_LOG_APPEND_TIME)
                    tsname = "log append time";
                std::cout << "Timestamp: " << tsname << " " << ts.timestamp << std::endl;
            }
            fprintf(stdout, "%% Message (topic %s [%d], "
                "offset %ld, %zd bytes):\n",
                message->topic_name().c_str(),
                message->partition(),
                message->offset(), message->len()
            );
            if (message->key_len()) {
                printf("Key: %.*s\n",
                    (int)message->key_len(), (char*)message->key_pointer());
            }
            printf("Message Payload: %.*s\n",
                (int)message->len(), (char*)message->payload());
            break;

        case RdKafka::ERR__PARTITION_EOF:
            // last message
            break;

        default:
            std::cerr << "Consume failed: " << message->errstr() << std::endl;
            run = false;
    }
}

int main()
{
    std::string brokers = "localhost";
    std::string errstr;
    std::vector<std::string> topics = {"mytopic"};

    std::unique_ptr<RdKafka::Conf> conf {
        RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)
    };
    std::unique_ptr<RdKafka::Conf> tconf {
        RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC)
    };

    ExampleEventCb ex_event_cb;
    RdKafka::Conf::ConfResult res = RdKafka::Conf::CONF_UNKNOWN;
    res = conf->set("metadata.broker.list", brokers, errstr);
    if (res == RdKafka::Conf::CONF_OK)
        res = conf->set("event_cb", &ex_event_cb, errstr);
    if (res == RdKafka::Conf::CONF_OK)
        res = conf->set("group.id", "default", errstr);
    if (res == RdKafka::Conf::CONF_OK)
        res = tconf->set("offset.store.method", "broker", errstr);
    if (res == RdKafka::Conf::CONF_OK)
        res = conf->set("default_topic_conf", tconf.get(), errstr);

    if (res != RdKafka::Conf::CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr.c_str());
        return EXIT_FAILURE;
    }

    signal(SIGINT, stop);

    std::unique_ptr<RdKafka::KafkaConsumer> consumer {
        RdKafka::KafkaConsumer::create(conf.get(), errstr)
    };
    if (!consumer.get()) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    std::cout << "% Created consumer " << consumer->name() << std::endl;

    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if (err) {
        std::cerr << "Failed to subscribe to " << topics.size() << " topics: "
                << RdKafka::err2str(err) << std::endl;
        exit(1);
    }
    while (run) {
        std::unique_ptr<RdKafka::Message> msg {
            consumer->consume(1000)
        };
        msg_consume(msg.get(), NULL);
    }

    consumer->close();

    /*
    * Wait for RdKafka to decommission.
    * This is not strictly needed (with check outq_len() above), but
    * allows RdKafka to clean up all its resources before the application
    * exits so that memory profilers such as valgrind wont complain about
    * memory leaks.
    */
    RdKafka::wait_destroyed(5000);
    return 0;
}
