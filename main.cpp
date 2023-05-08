#include <iostream>
#include <memory>
// #include <stdexcept>
#include <string>
#include <array>
#include <queue>
#include <thread>
#include <mutex>
#include <queue>
std::queue<std::string> log_queue;
// static std::mutex log_mutex;

void pg_logical_init();
void exec_queue(const char* commend);

int main() {
    char log_commend[256] ="pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -";
    pg_logical_init();
    std::thread get_log(exec_queue,log_commend);
    // exec_queue("pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -");
    while (1) {
        if(!log_queue.empty()){
            std::cout<<"read queue!:"<<log_queue.front();
            log_queue.pop();
        }

    }
    get_log.join();
}

void pg_logical_init(){
    system("pg_recvlogical -d postgres --drop-slot --slot test_slot");
    system("pg_recvlogical -d postgres --slot test_slot --create-slot -P wal2json");
}

void exec_queue(const char* commend) {
    const int buffer_size=256;
    std::array<char, buffer_size> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commend, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {

        // std::cout<<buffer.data();
            std::cout<<"log: push to queue!"<<std::endl;
            log_queue.push(buffer.data());
        // if(log_mutex.try_lock()){
        //     log_mutex.unlock();
        // } else {
        //     //throww error
        //     throw std::runtime_error("mutex: try_lock failed!");
        // }
    }

}