#include <map>
#include <string>
std::string pg_logical_init(std::string db_user);
char *broker_cat(std::string ip, std::string port);
std::string log_parser(std::string, std::string);
void logger_writer(int id,std::string message,std::string path);
class ConfigParser
{
public:
    ConfigParser(const std::string &path);
    std::string GetValue(const std::string &name);
    bool GetBool(const std::string &name);
    void ConfigTest();

private:
    std::map<std::string, std::string> conf_table;
};
