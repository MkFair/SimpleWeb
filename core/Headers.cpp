#include "Headers.h"
Headers::Headers(std::string raw_headers,std::string client_ip)
    :raw_headers_(raw_headers)
{
    valmap["CLIENT_IP"] = client_ip;
}
std::map<std::string,std::string> Headers::get_headers(){
    valmap = by_regex(raw_headers_);
    return valmap;
}
std::map<std::string,std::string> Headers::by_regex(std::string raw_headers){
    std::regex rule_header("([^\n]*?): ([^\r]*?)",std::regex::extended);
    std::smatch m;
    std::map<std::string,std::string> headers;
    headers = parse_start_line(raw_headers);
    std::stringstream ss(raw_headers);
    std::string line;
    while( std::getline(ss,line,'\n') ){
        if(std::regex_search(line,m,rule_header,std::regex_constants::match_any)){
            if(m.size()==3){
                headers[m[1].str()]=m[2].str();
            }
        }
    }
    return headers;
}
std::map<std::string,std::string> Headers::parse_start_line(std::string raw_start_line){
//формат стартовой линии
		std::regex start_line_format("(GET|POST{1})\\s([^\\s]*?)\\sHTTP/(\\d\\.\\d)\\r\\n");
		std::smatch m;
		std::map<std::string,std::string>start_line_result;
		//если по рег выражению есть совпадение с форматом стартовой линии
		if(std::regex_search(raw_start_line,m,start_line_format)){
			if( m.size() > 3 ){
				start_line_result["METHOD"] = m[1].str();
                std::string spart(m[2]);
                uint pos = spart.find("?");
                if(pos!=std::string::npos and pos<spart.size() ){
                    start_line_result["REQUEST_URN"] = spart.substr(0,pos);
                }else{
                    start_line_result["REQUEST_URN"] = spart;
                }
				start_line_result["REQUEST_URI"] = m[2].str();
				start_line_result["HTTP_VERSION"] = m[3].str();
			}
		}
		return start_line_result;
}
