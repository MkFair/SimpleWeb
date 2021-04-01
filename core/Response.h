#include<map>
#include<string>

class Response{
    std::map<std::string,std::string> headers_;
    std::string body_;
    std::string status_label_;
    uint satus_code_;
    public:
    Response();
    ~Response();
    std::string generate();
    void set_header(std::string,std::string);
    std::string raw_page();
    void set_body(std::string);
    void set_status_code(uint code);
};
