#include "function.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <map>
#include <filesystem>

std::string log_parser(std::string json, std::string key)
{
    Poco::JSON::Parser loParser;
    Poco::Dynamic::Var parse = loParser.parse(json);
    Poco::JSON::Object::Ptr pObject = parse.extract<Poco::JSON::Object::Ptr>();
    return pObject->getValue<std::string>(key);
}
char *broker_cat(std::string ip, std::string port)
{
    char *broker_ip = new char[std::strlen(ip.c_str())];
    char *broker_port = new char[std::strlen(port.c_str())];
    std::strcpy(broker_ip, ip.c_str());
    std::strcpy(broker_port, port.c_str());

    char *broker = new char[std::strlen(broker_ip) + std::strlen(broker_port) + 1];
    std::strcpy(broker, broker_ip);
    std::strcat(broker, ":");
    std::strcat(broker, broker_port);
    return broker;
}
void logger_writer(int id,std::string message,std::string path){
    // -1:error, 0:start log, 1:down log, 2:warning log, 3:system log,
    std::ofstream file;
    std::string content;
    time_t bf_time;
    time(&bf_time);
    std::string now_time = asctime(localtime(&bf_time));
    
    content= now_time.substr(0,now_time.length()-1)+", ";
    if(id==-1){
        content += "[Err], ";
    } else if (id==0){
        content += "[Start], ";
    } else if(id==1){
        content += "[Down], ";
    } else if(id==2){
        content += "[Warning], ";
    } else if(id==3){
        content += "[Syslog], ";
    } else {
        content += "[Unknown], ";
    }
    content+=message+'\n';
    file.open(path+"/log.txt", std::ios_base::app);
    file<<content;
    file.close();
    return;
}
std::string pg_logical_init(std::string db_user)
{
    // system 안에 db_user 적용
    system("pg_recvlogical -d ubuntu --drop-slot --slot test_slot");
    system("pg_recvlogical -d ubuntu --slot test_slot --create-slot -P wal2json");
    std::string output = "pg_recvlogical -d ubuntu --slot test_slot --start -o format-version=2 -o include-lsn=true -o include-timestamp=true -o add-msg-prefixes=wal2json --file -";
    
    return output;
}
ConfigParser::ConfigParser(const std::string &path)
{
    // if not file exist ? throw error

    std::ifstream openFile(path);
    
    if (openFile.is_open())
    {
        std::string line;
        while (getline(openFile, line))
        {
            std::string delimiter = " = ";
            if (std::string::npos == line.find(delimiter))
                delimiter = "=";
            std::string key = line.substr(0, line.find(delimiter));
            std::string value = line.substr(line.find(delimiter) + delimiter.length(), line.length());
            conf_table[key] = value;
        }
        openFile.close();
    }
    else
    {
        throw std::runtime_error("Could not open file");
    }
}

std::string ConfigParser::GetValue(const std::string &name)
{
    if (conf_table.find(name) != conf_table.end())
    {
        if (conf_table[name].find("\"") == std::string::npos)
        {
            return conf_table[name];
        }
        else
        {
            return conf_table[name].substr(1, conf_table[name].length() - 2);
        }
    }
    else
    {
        throw std::invalid_argument("key not exeist.");
    }
}
bool ConfigParser::GetBool(const std::string &name)
{
    std::string val = GetValue(name);
    if (val.compare("T") == 0 || val.compare("t") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void ConfigParser::ConfigTest()
{
    std::cout << "info: print config information." << std::endl;
    for (auto i : conf_table)
    {
        std::cout << i.first << " : " << i.second << std::endl;
    }
}