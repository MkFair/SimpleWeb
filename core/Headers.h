#include <string>
#include <map>
#include <regex>
#include <iterator>
class Headers{
    std::map<std::string,std::string>valmap;
    std::string raw_headers_;
    public:
    Headers(std::string);
    std::map<std::string,std::string> by_regex(std::string);
    std::map<std::string,std::string> parse_start_line(std::string raw_start_line);
    std::map<std::string,std::string> get_headers();
    ~Headers(){};
};
