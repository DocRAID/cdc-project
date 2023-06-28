#include "fun_kafka.h"
#include <librdkafka/rdkafka.h>

static void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque)
{
    if (rkmessage->err)
    {
        fprintf(stderr, "%% Message delivery failed: %s\n", rd_kafka_err2str(rkmessage->err));
    }
    else
    {
        fprintf(stderr, "%% Message delivered (%zd bytes, "
                        "partition %" PRId32 ")\n",
                rkmessage->len, rkmessage->partition);
    }
}
