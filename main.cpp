#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <queue>
#include <thread>
#include <mutex>
#include <queue>
#include <unistd.h>
#include "function.h"
std::queue<std::string> log_queue;
// static std::mutex log_mutex;

void exec_queue(const char* commend);

int main(int argc,char* argv[]) {
    //config parser test 기능추가 하고 하면 될듯.
    ConfigParser config_info(argv[1]);
    // std::cout<<config_info.GetValue("lim")<<std::endl;

    //todo : pg_logical init 안전한 방법 찾기, signal 처리 하기.
    pg_logical_init(config_info.GetValue("targitDbUser"));
    char log_commend[256] ="pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -";
    std::thread get_log(exec_queue,log_commend);
    // exec_queue("pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -");
    while (1) {
        if(!log_queue.empty()) {
            std::string action = log_parser(log_queue.front(),"action");

            if(action.compare("B")==0) {
                //무언가가 일어나긴 함.
                // std::cout<<log_parser(log_queue.front(),"lsn")<<std::endl;
            } else if(action.compare("I")==0) {
                //insert
                std::cout<<log_parser(log_queue.front(),"columns")<<std::endl;
                //lsn 기록 log_parser(log_queue.front(),"lsn")
            } else if (action.compare("T")==0) {
                //truncate
                //lsn 기록 log_parser(log_queue.front(),"lsn")
            } else {
                std::cout<<log_parser(log_queue.front(),"action")<<std::endl;
            }
            // std::cout<<log_queue.front();
            // sleep(1);
            log_queue.pop();
        }

    }
    get_log.join();
    // 프로그램이 종료될때 slot삭제하고, 로그 남기고 종료.
}

void exec_queue(const char* commend) {
    const int buffer_size=1024;
    std::array<char, buffer_size> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commend, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        // std::cout<<buffer.data();
        // if(log_mutex.try_lock()){
        log_queue.push(buffer.data());
        //     log_mutex.unlock();
        // } else {
        //     //throww error
        //     throw std::runtime_error("mutex: try_lock failed!");
        // }
    }

}