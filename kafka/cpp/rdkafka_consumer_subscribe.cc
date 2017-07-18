#include <librdkafka/rdkafka.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static int run = 1;
static void stop(int sig)
{
    run = 0;
    fclose(stdin);
}

static void msg_consume(rd_kafka_message_t *rkmessage, void *opaque)
{
    if (rkmessage->err) {
        if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF) {
            fprintf(stderr,
                "%% Consumer reached end of %s [%" PRId32 "] "
                "message queue at offset %" PRId64 "\n",
                rd_kafka_topic_name(rkmessage->rkt),
                rkmessage->partition, rkmessage->offset
            );
            return;
        }
        if (rkmessage->rkt) {
            fprintf(stderr, "%% Consumer error for "
                "topic \"%s\" [%" PRId32 "] "
                "offset %" PRId64 ": %s\n",
                rd_kafka_topic_name(rkmessage->rkt),
                rkmessage->partition, rkmessage->offset,
                rd_kafka_message_errstr(rkmessage)
            );
        } else {
            fprintf(stderr, "%% Consumer error: %s: %s\n",
                rd_kafka_err2str(rkmessage->err),
                rd_kafka_message_errstr(rkmessage)
            );
        }
        if (rkmessage->err == RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION ||
            rkmessage->err == RD_KAFKA_RESP_ERR__UNKNOWN_TOPIC) {
            run = 0;
        }
        return;
    }
    fprintf(stdout, "%% Message (topic %s [%" PRId32 "], "
        "offset %" PRId64 ", %zd bytes):\n",
        rd_kafka_topic_name(rkmessage->rkt),
        rkmessage->partition,
        rkmessage->offset, rkmessage->len
    );
    if (rkmessage->key_len) {
        printf("Key: %.*s\n",
               (int)rkmessage->key_len, (char*)rkmessage->key);
    }
    printf("Message Payload: %.*s\n",
           (int)rkmessage->len, (char*)rkmessage->payload);
}

int main()
{
    const char *brokers = "localhost:9092";
    char errstr[512];
    rd_kafka_conf_t *conf = rd_kafka_conf_new();
    rd_kafka_topic_conf_t *topic_conf = rd_kafka_topic_conf_new();

    rd_kafka_conf_res_t res = RD_KAFKA_CONF_UNKNOWN;
    /* set rd_kafka conf and topic conf
    char *name, *value;
    res = rd_kafka_conf_set(conf, name, value, errstr, sizeof(errstr));
    res = rd_kafka_topic_conf_set(topic_conf, "auto.commit.enable", "true", errstr, sizeof(errstr));
    res = rd_kafka_topic_conf_set(topic_conf, "auto.commit.interval.ms", "1000", errstr, sizeof(errstr));
    // if auto.commit.enable = false, call rd_kafka_offset_store()
    */

    signal(SIGINT, stop);

    // If get from offset
    {
        const char *group = "default";
        res = rd_kafka_conf_set(conf, "group.id", group, errstr, sizeof(errstr));
        if (res != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%% %s\n", errstr);
            return EXIT_FAILURE;
        }

        // consumer groups alawys use broker based offset storage
        // offset store support two mode: "broker" and "local", see https://github.com/edenhill/librdkafka/wiki/Consumer-offset-management
        res = rd_kafka_topic_conf_set(topic_conf, "offset.store.method", "broker", errstr, sizeof(errstr));
        if (res != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%% %s\n", errstr);
            return EXIT_FAILURE;
        }

        // set default topic config for pattern-matched topics
        rd_kafka_conf_set_default_topic_conf(conf, topic_conf);

        // rd_kafka_conf_set_rebalance_cb(conf, rebalance_cb);
    }

    // create kafka handle
    rd_kafka_t *rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
        return EXIT_FAILURE;
    }
    // add brokers
    if (rd_kafka_brokers_add(rk, brokers) == 0) {
        fprintf(stderr, "%% No valid brokers specified\n");
        return EXIT_FAILURE;
    }

    // redirect rd_kafka_poll() to consumer_poll()
    rd_kafka_poll_set_consumer(rk);

    struct {
        const char *topic;
        int partition;
    } topicConf[] = {{"mytopic", -1}};
    int topicConfSize = sizeof(topicConf) / sizeof(topicConf[0]);
    rd_kafka_topic_partition_list_t *topics;
    topics = rd_kafka_topic_partition_list_new(topicConfSize);
    for (int i = 0; i < topicConfSize; ++i) {
        rd_kafka_topic_partition_list_add(topics, topicConf[i].topic, topicConf[i].partition);
    }

    fprintf(stderr, "%% Subscribing to %d topics\n", topics->cnt);
    rd_kafka_resp_err_t err;
    err = rd_kafka_subscribe(rk, topics);
    if (err) {
        fprintf(stderr, "%% Failed to start consuming topics: %s\n", rd_kafka_err2str(err));
        return EXIT_FAILURE;
    }

    while (run) {
        rd_kafka_message_t *rkmessage;
        rkmessage = rd_kafka_consumer_poll(rk, 1000);
        if (rkmessage) {
            msg_consume(rkmessage, NULL);
            rd_kafka_message_destroy(rkmessage);
        }
    }

    err = rd_kafka_consumer_close(rk);
    if (err) {
        fprintf(stderr, "%% Failed to close consumer: %s\n", rd_kafka_err2str(err));
    } else {
        fprintf(stderr, "%% Consumer closed\n");
    }
    rd_kafka_topic_partition_list_destroy(topics);
    rd_kafka_destroy(rk);

    /* Let background threads clean up and terminate cleanly. */
    run = 5;
    while (run-- > 0 && rd_kafka_wait_destroyed(1000) == -1)
        printf("Waiting for librdkafka to decommission\n");
    if (run <= 0)
        rd_kafka_dump(stdout, rk);

    return 0;
}
