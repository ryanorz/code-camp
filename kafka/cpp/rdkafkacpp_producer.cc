#include <librdkafka/rdkafkacpp.h>
#include <string>
#include <iostream>
#include <signal.h>

using namespace std;

static bool run = true;

static void stop(int sig)
{
    run = false;
    fclose(stdin);
}

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message & message) override
    {
        if (message.err()) {
            fprintf(stderr, "%% Message delivery failed: %s\n",
                message.errstr().c_str()
            );
        } else {
            fprintf(stderr, "%% Message delivered (%zd bytes, partition %d)\n",
                message.len(), message.partition()
            );
        }
    }
};

int main()
{
    string errstr;
    const char *brokers = "localhost:9092";
    const char *topic = "mytopic";

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    if (RdKafka::Conf::ConfResult::CONF_OK != conf->set("bootstrap.servers", brokers, errstr)) {
        fprintf(stderr, "%% Failed to set conf: %s", errstr.c_str());
        return EXIT_FAILURE;
    }

    // set the delivery report callback
    ExampleDeliveryReportCb ex_dr_cb;
    conf->set("dr_cb", &ex_dr_cb, errstr);

    // create producer instance
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr.c_str());
        return EXIT_FAILURE;
    }

    RdKafka::Topic *rkt = RdKafka::Topic::create(producer, topic, NULL, errstr);
    if (!rkt) {
        fprintf(stderr, "%% Failed to create topic object: %s\n", errstr.c_str());
        delete producer;
        return EXIT_FAILURE;
    }

    signal(SIGINT, stop);

    fprintf(stderr,
        "%% Type some text and hit enter to produce message\n"
        "%% Or just hit enter to only serve delivery reports\n"
        "%% Press Ctrl-C or Ctrl-D to exit\n"
    );

    for (std::string line; run && std::getline(std::cin, line);) {
        if (line.empty()) {
            // Empty line: only serve delivery reports
            producer->poll(0);
            continue;
        }

        // send/produce message
        // asynchronous call.
retry:
        RdKafka::ErrorCode resp = producer->produce(
            rkt,
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
            // payload
            const_cast<char *>(line.c_str()), line.size(),
            // key
            NULL,
            // opaque
            NULL
        );
        if (resp != RdKafka::ERR_NO_ERROR) {
            fprintf(stderr, "%% failed to produce to topic %s : %s\n",
                rkt->name().c_str(),
                RdKafka::err2str(resp).c_str()
            );
            // poll to handle delivery reports
            if (resp == RdKafka::ERR__QUEUE_FULL) {
                // If internal queue is full, wait for message to be delivered
                // and then retry
                // internal queue is limited by queue.buffering.max.message
                producer->poll(1000/*block for max 1000ms*/);
                goto retry;
            }
        } else {
            fprintf(stderr, "%% Enqueued message (%zd bytes) for topic %s\n",
                line.size(), rkt->name().c_str()
            );
        }
        // rd_kafka_poll()放在主循环中或这专门的线程中
        // 确保每个 rd_kafka_produce() 都调用 rd_kafka_poll()
        producer->poll(0);
    }
    // wait for final message to be delivered or fail.
    fprintf(stderr, "%% Flushing final messages..\n");
    producer->flush(5*1000/*wait for 5 seconds*/);
    // destroy objects
    delete rkt;
    delete producer;
    return EXIT_SUCCESS;
}
