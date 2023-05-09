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
    //config parser test
    ConfigParser config_file(argv[1]);
    std::cout<<config_file.GetValue("lim")<<std::endl;
    std::cout<<config_file.GetBool("bool")<<std::endl;

    //todo : pg_logical init 안전한 방법 찾기, signal 처리 하기.
    char log_commend[256] ="pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -";
    pg_logical_init();
    std::thread get_log(exec_queue,log_commend);
    // exec_queue("pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -");
    while (1) {
        if(!log_queue.empty()){
            std::cout<<"read queue!:"<<lsn_parser(log_queue.front())<<std::endl;
            // std::cout<<log_queue.front()<<std::endl;
            sleep(1);
            log_queue.pop();
        }

    }
    get_log.join();
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
            log_queue.push(buffer.data());
        // if(log_mutex.try_lock()){
        //     log_mutex.unlock();
        // } else {
        //     //throww error
        //     throw std::runtime_error("mutex: try_lock failed!");
        // }
    }

}