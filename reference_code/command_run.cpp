#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

void exec(const char* commend) {
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commend, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::cout<<buffer.data();
    }

}
int main() {
    exec("pg_recvlogical -d postgres --slot test_slot --start -o format-version=2 -o include-lsn=true -o add-msg-prefixes=wal2json --file -");
}