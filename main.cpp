#include <iostream>
#include <memory>
#include <cstring>
#include <array>
#include <queue>
#include <thread>
#include <mutex>
#include <queue>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include "function.h"
#include "fun_kafka.h"
#include <librdkafka/rdkafka.h>

static volatile sig_atomic_t run = 1;

static void stop(int sig)
{
    
    run = 0;
    fclose(stdin);
}

std::queue<std::string> log_queue;
std::queue<std::string> kafka_msg_queue;

void exec_queue_th(const char *commend);
void kafka_proc_th(const char *broker, const char *topic);

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "%% Usage: %s <config-ini>\n", argv[0]);
        return 1;
    }
    ConfigParser config_info(argv[1]);
    config_info.ConfigTest();

    char log_commend[256];
    std::strcpy(log_commend,pg_logical_init(config_info.GetValue("source-db-user")).c_str());
    
    // prepare broker and topic
    char *broker = broker_cat(config_info.GetValue("kafka-ip"), config_info.GetValue("kafka-port"));
    char *topic = new char[std::strlen(config_info.GetValue("kafka-topic").c_str())];
    std::strcpy(topic,config_info.GetValue("kafka-topic").c_str());
    // 로그를 가져오는 스레드 생성
    std::thread exec_queue(exec_queue_th, log_commend);
    // kafka에 적용시키는 스레드 생성
    std::thread kafka_proc(kafka_proc_th, broker, topic);
    // exec_queue("pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -");
    signal(SIGINT, stop);
    logger_writer(0,"server start",config_info.GetValue("export-log-path"));
    while (run)
    {
        if (!log_queue.empty())
        {
            std::string action = log_parser(log_queue.front(), "action");
            if (action.compare("C") == 0)
            {
                // commit log
                //  std::cout<<log_parser(log_queue.front(),"lsn")<<std::endl;
                kafka_msg_queue.push("commit: " + log_parser(log_queue.front(), "lsn"));
            }
            else if (action.compare("I") == 0)
            {
                // insert
                //  std::cout<<log_parser(log_queue.front(),"columns")<<std::endl;
                kafka_msg_queue.push("insert: " + log_parser(log_queue.front(), "columns"));
                // lsn 기록 log_parser(log_queue.front(),"lsn")
            }
            else if (action.compare("D") == 0)
            {
                // delete
                //  std::cout<<log_parser(log_queue.front(),"columns")<<std::endl;
                kafka_msg_queue.push("delete: " + log_parser(log_queue.front(), "columns"));
                // lsn 기록 log_parser(log_queue.front(),"lsn")
            }
            else if (action.compare("U") == 0)
            {
                // update
                //  std::cout<<log_parser(log_queue.front(),"columns")<<std::endl;
                kafka_msg_queue.push("update: " + log_parser(log_queue.front(), "columns"));
                // lsn 기록 log_parser(log_queue.front(),"lsn")
            }
            else if (action.compare("T") == 0)
            {
                // truncate
                kafka_msg_queue.push("tuncat: " + log_parser(log_queue.front(), "columns"));
            }
            else
            {
                kafka_msg_queue.push("warning: unknown option");
            }
            std::cout << log_queue.front() << std::endl;
            log_queue.pop();
        }
    }
    logger_writer(1,"server down",config_info.GetValue("export-log-path"));
    exec_queue.join();
    kafka_proc.join();
    //log:shutdown
    // 프로그램이 종료될때 slot삭제하고, 로그 남기고 종료.
}

void exec_queue_th(const char *commend)
{
    const int buffer_size = 1024;
    std::array<char, buffer_size> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commend, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (run && fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        log_queue.push(buffer.data());
    }
}

void kafka_proc_th(const char *broker, const char *topic)
{
    std::cout << "server be connect with broker: " << broker << ", topic:" << topic << std::endl;
    rd_kafka_t *rk;
    rd_kafka_conf_t *conf;
    char errstr[512];
    char buf[1024];

    conf = rd_kafka_conf_new();

    if (rd_kafka_conf_set(conf, "bootstrap.servers", broker, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
    {
        fprintf(stderr, "%s\n", errstr);
        return;
    }

    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk)
    {
        fprintf(stderr, "%% Failed to create new producer: %s\n",
                errstr);
        return;
    }

    while (run)
    {
        if (!kafka_msg_queue.empty())
        {
            // buf=kafka_msg_queue.front().c_str();
            strcpy(buf, kafka_msg_queue.front().c_str());
            // queue에서 뽑아온거
            size_t len = strlen(buf);
            rd_kafka_resp_err_t err;

            if (buf[len - 1] == '\n') /* Remove newline */
                buf[--len] = '\0';

            if (len == 0)
            {
                /* Empty line: only serve delivery reports */
                rd_kafka_poll(rk, 0 /*non-blocking */);
                continue;
            }

        retry:
            err = rd_kafka_producev(
                rk,
                RD_KAFKA_V_TOPIC(topic),
                RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                RD_KAFKA_V_VALUE(buf, len),
                RD_KAFKA_V_OPAQUE(NULL),
                RD_KAFKA_V_END);

            if (err)
            {
                fprintf(stderr, "%% Failed to produce to topic %s: %s\n", topic, rd_kafka_err2str(err));

                if (err == RD_KAFKA_RESP_ERR__QUEUE_FULL)
                {
                    rd_kafka_poll(rk, 1000);
                    goto retry;
                }
            }
            else
            {
                fprintf(stderr, "%% Enqueued message (%zd bytes) for topic %s\n", len, topic);
            }
            rd_kafka_poll(rk, 0);
            memset(buf, 0, sizeof(buf));
            kafka_msg_queue.pop();
        }
    }
    fprintf(stderr, "%% Flushing final messages..\n");
    rd_kafka_flush(rk, 10 * 1000);
    if (rd_kafka_outq_len(rk) > 0)
    {
        fprintf(stderr, "%% %d message(s) were not delivered\n", rd_kafka_outq_len(rk));
    }

    rd_kafka_destroy(rk);

    return;
}