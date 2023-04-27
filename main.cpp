#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <set>
#include <iostream>
#include<vector>
#include<fstream>

std::vector<std::string>file_read_on(std::string path);
std::vector<std::string>lsn_parser(std::vector<std::string> json);
int main() {
    std::vector<std::string> json = file_read_on("/home/postgres/test.json");
    for(auto i:json) {
        std::cout<<i<<std::endl;
    }
    for(auto i:lsn_parser(json)){
        std::cout<<i<<std::endl;
    }
    std::cout<<"exit"<<std::endl;
}

std::vector<std::string>lsn_parser(std::vector<std::string> json){
    std::vector<std::string> result;
    Poco::JSON::Parser loParser;
    for(auto i:json) {
        Poco::Dynamic::Var parse = loParser.parse(i);
        Poco::JSON::Object::Ptr pObject = parse.extract<Poco::JSON::Object::Ptr>();
        result.push_back(pObject->getValue<std::string>("lsn"));
        // pObject->getValue<std::string>("lsn");
    }
    return result;
}

std::vector<std::string> file_read_on(std::string path){
    std::vector<std::string> result;
    std::ifstream openFile(path.data());
    if(openFile.is_open()){
        std::string line;
        while(getline(openFile,line)){
            result.push_back(line);
            // std::cout<<line<<std::endl;
        }
        openFile.close();
    } else {
        std::cout<<"error"<<std::endl;
    }
    return result;
}