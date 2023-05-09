#include<string>
#include<map>
void pg_logical_init();
std::string lsn_parser(std::string);

class ConfigParser {
    public:
    ConfigParser(const std::string &path);
    std::string GetValue(const std::string& name);
    bool GetBool(const std::string& name);
    private:
    std::map<std::string, std::string> conf_table;
};