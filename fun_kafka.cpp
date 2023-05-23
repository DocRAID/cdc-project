//https://docs.confluent.io/kafka-clients/librdkafka/current/overview.html
#include "fun_kafka.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "librdkafka/rdkafka.h"
void kafka_init() {
    char hostname[128];
    char errstr[512];

    rd_kafka_conf_t *conf = rd_kafka_conf_new();

    if (gethostname(hostname, sizeof(hostname))) {
        fprintf(stderr, "%% Failed to lookup hostname\n");
        exit(1);
    }

    if (rd_kafka_conf_set(conf, "client.id", hostname,
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    if (rd_kafka_conf_set(conf, "bootstrap.servers", "host1:9092,host2:9092",
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    rd_kafka_topic_conf_t *topic_conf = rd_kafka_topic_conf_new();

    if (rd_kafka_topic_conf_set(topic_conf, "acks", "all",
                                errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        exit(1);
    }

    /* Create Kafka producer handle */
    rd_kafka_t *rk;
    if (!(rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf,
                            errstr, sizeof(errstr)))) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        exit(1);
    }
    //asdf
    std::string topic
    rd_kafka_topic_t *rkt = rd_kafka_topic_new(rk, topic, topic_conf);

    if (rd_kafka_produce(rkt, RD_KAFKA_PARTITION_UA, RD_KAFKA_MSG_F_COPY, payload, payload_len, key, key_len, NULL) == -1) {
        fprintf(stderr, "%% Failed to produce to topic %s: %s\n",
                topic, rd_kafka_err2str(rd_kafka_errno2err(errno)));
    }


}

static void on_delivery(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage,void *opaque) {
    if (rkmessage->err)
        fprintf(stderr, "%% Message delivery failed: %s\n",
                rd_kafka_message_errstr(rkmessage));
}

void init_rd_kafka() {
    rd_kafka_conf_t *conf = rd_kafka_conf_new();
    rd_kafka_conf_set_dr_msg_cb(conf, on_delivery);

    // initialization omitted
}