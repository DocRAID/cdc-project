#include "function.h"
#include <iostream>
#include <fstream>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <map>
std::string log_parser(std::string json,std::string key) {
    Poco::JSON::Parser loParser;
    Poco::Dynamic::Var parse = loParser.parse(json);
    Poco::JSON::Object::Ptr pObject = parse.extract<Poco::JSON::Object::Ptr>();
    return pObject->getValue<std::string>(key);
}
void pg_logical_init(std::string db_user) {
    //system 안에 db_user 적용
    system("pg_recvlogical -d postgres --drop-slot --slot test_slot");
    system("pg_recvlogical -d postgres --slot test_slot --create-slot -P wal2json");
}
ConfigParser::ConfigParser(const std::string& path)
{
    // if not file exist ? throw error
    std::ifstream openFile(path);
    if (openFile.is_open()) {
        std::string line;
        while (getline(openFile, line)) {
            std::string delimiter = " = ";
            if (std::string::npos == line.find(delimiter)) delimiter = "=";
            std::string key = line.substr(0, line.find(delimiter));
            std::string value = line.substr(line.find(delimiter) + delimiter.length(), line.length());
            conf_table[key] = value;
        }
        openFile.close();
    } else {
        throw std::runtime_error("Could not open file");
    }
}

std::string ConfigParser::GetValue(const std::string& name) {
    if(conf_table.find(name) != conf_table.end()) {
        if (conf_table[name].find("\"") == std::string::npos) {
            return conf_table[name];
        }
        else {
            return conf_table[name].substr(1, conf_table[name].length() - 2);
        }
    } else {
        throw std::invalid_argument("key not exeist.");
    }
}
bool ConfigParser::GetBool(const std::string& name) {
    std::string val = GetValue(name);
    if(val.compare("T")==0||val.compare("t")==0) {
        return true;
    } else {
        return false;
    }
}
void ConfigParser::ConfigTest(){
    std::cout<<"info: print config information."<<std::endl;
    for(auto i : conf_table){
        std::cout<<i.first<<" : "<<i.second<<std::endl;
    }
}