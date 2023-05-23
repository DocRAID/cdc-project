void kafka_init();
static void on_delivery(rd_kafka_t*, const rd_kafka_message_t*,void*);
void init_rd_kafka();