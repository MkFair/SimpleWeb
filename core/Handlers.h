#ifndef HANDLERS_HPP_
#define HANDLERS_HPP_
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <jsoncpp/json/writer.h>


class FileNotFound:std::exception{};

class RequestHandler{
protected:
	std::map<std::string,std::string> headers_;
    std::map<std::string,std::string> get_params;
public:
	RequestHandler(){}
    virtual void set_headers(std::map<std::string,std::string> headers);
    virtual std::string build_resp(){return std::string();};
	virtual ~RequestHandler(){};
};
class ApiHandler : public RequestHandler{
private:
    std::string v = "0.0.1";
    
public:
	std::string resp_;
    std::map<int,std::string> params_by_key;
	ApiHandler() : RequestHandler(){
		clear_headers();
	}
    void clear_headers(){
        resp_ = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nKeep-Alive: Close\r\n\r\n";
    }
	virtual Json::Value handler() {return Json::Value();}
    std::string build_resp() override{
        clear_headers();
        Json::Value json_vals = handler();
        json_vals["version"] = v;
        Json::StreamWriterBuilder writer;
        std::string s = resp_ + Json::writeString(writer,json_vals);
        return s;
    };
	virtual ~ApiHandler(){};
};

class StaticHandler : public RequestHandler{
public:
	StaticHandler() : RequestHandler(){}
	virtual std::string handler();
    virtual std::string build_resp() override {return handler();};
	virtual ~StaticHandler(){};
};
class TemplateHandler : public RequestHandler{
    std::string filename_;
public:
	TemplateHandler(std::string filename) : RequestHandler(),filename_(filename){}
	virtual std::string handler();
    virtual std::string build_resp() override {return handler();};
	virtual ~TemplateHandler(){};
};



#endif /* HANDLERS_HPP_ */
