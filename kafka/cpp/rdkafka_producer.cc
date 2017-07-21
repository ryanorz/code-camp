#include <librdkafka/rdkafka.h>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

static int run = 1;

static void stop(int sig)
{
    run = 0;
    fclose(stdin);
}

// callback is called per messge.
// Success: rkmessage->ERR == RD_KAFKA_RESP_ERR_NO_ERROR
// callback is triggered from rd_kafka_poll()
static void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque)
{
    if (rkmessage->err) {
        fprintf(stderr, "%% Message delivery failed: %s\n",
            rd_kafka_err2str(rkmessage->err)
        );
    } else {
        fprintf(stderr, "%% Message delivered (%zd bytes, partition %" PRId32 ")\n",
            rkmessage->len, rkmessage->partition
        );
    }
    // rkmessage is destroyed automatically by librdkafka
}

int main()
{
    rd_kafka_t *rk;             // producer instance handle
    rd_kafka_topic_t *rkt;      // topic object
    rd_kafka_conf_t *conf;      // temporary config object
    char errstr[512];
    char buf[512];

    const char *brokers = "localhost:9092";
    const char *topic = "mytopic";

    conf = rd_kafka_conf_new();
    if (RD_KAFKA_CONF_OK != rd_kafka_conf_set(conf,
        "bootstrap.servers", brokers, errstr, sizeof(errstr))) {
        fprintf(stderr, "%% Failed to set conf: %s", errstr);
        return EXIT_FAILURE;
    }
    // set the delivery report callback
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // create producer instance
    //NOTE 在这里使用了conf后，它不能被程序再次引用
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return EXIT_FAILURE;
    }

    // rk 和 rkt 都是长时间存在的对象， 应该尽可能的复用
    rkt = rd_kafka_topic_new(rk, topic, NULL);
    if (!rkt) {
        fprintf(stderr, "%% Failed to create topic object: %s\n",
            rd_kafka_err2str(rd_kafka_last_error())
        );
        rd_kafka_destroy(rk);
        return EXIT_FAILURE;
    }

    signal(SIGINT, stop);

    fprintf(stderr,
        "%% Type some text and hit enter to produce message\n"
        "%% Or just hit enter to only serve delivery reports\n"
        "%% Press Ctrl-C or Ctrl-D to exit\n"
    );

    while(run && fgets(buf, sizeof(buf), stdin)) {
        size_t len = strlen(buf);
        if (buf[len-1] == '\n') // remove newline
            buf[--len] = '\0';
        if (len == 0) {
            // Empty line: only serve delivery reports
            rd_kafka_poll(rk, 0/*non-blocking*/);
            continue;
        }
        // send/produce message
        // asynchronous call.
retry:
        if (rd_kafka_produce(
            rkt,
            RD_KAFKA_PARTITION_UA,
            RD_KAFKA_MSG_F_COPY,
            // payload
            buf, len,
            // key
            NULL, 0,
            // opaque
            NULL
        ) == -1) {
            fprintf(stderr, "%% failed to produce to topic %s : %s\n",
                rd_kafka_topic_name(rkt),
                rd_kafka_err2str(rd_kafka_last_error())
            );
            // poll to handle delivery reports
            if (rd_kafka_last_error() == RD_KAFKA_RESP_ERR__QUEUE_FULL) {
                // If internal queue is full, wait for message to be delivered
                // and then retry
                // internal queue is limited by queue.buffering.max.message
                rd_kafka_poll(rk, 1000/*block for max 1000ms*/);
                goto retry;
            }
        } else {
            fprintf(stderr, "%% Enqueued message (%zd bytes) for topic %s\n",
                len, rd_kafka_topic_name(rkt)
            );
        }
        // rd_kafka_poll()放在主循环中或这专门的线程中
        // 确保每个 rd_kafka_produce() 都调用 rd_kafka_poll()
        rd_kafka_poll(rk, 0/*non-blocking*/);
    }
    // wait for final message to be delivered or fail.
    fprintf(stderr, "%% Flushing final messages..\n");
    rd_kafka_flush(rk, 5*1000/*wiat for 5 seconds*/);
    // destroy objects
    rd_kafka_topic_destroy(rkt);
    rd_kafka_destroy(rk);
    return 0;
}
