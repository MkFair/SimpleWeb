#include "Response.h"
Response::Response(){
    
}
Response::~Response(){
    
}
std::string Response::generate(){
    std::string generation_text = "HTTP/1.1 "+std::to_string(satus_code_)+" "+status_label_+"\r\n";
    for(auto header_line: headers_){
        generation_text+=header_line.first+" "+header_line.second+"\r\n";
    }
    generation_text+="\r\n\r\n";
    if(body_.size()>0){
        generation_text+=body_+"\r\n\r\n";
    }
    return generation_text;
}
void Response::set_body(std::string s){
    body_ = s;
}
void Response::set_header(std::string key,std::string value){
    headers_[key] = value;
}
void Response::set_status_code(uint code){
    satus_code_ =  code;
    switch(code){
        case 400:
            status_label_="Not Found";
            break;
        case 200:
            status_label_="OK";
            break;
    }
}
