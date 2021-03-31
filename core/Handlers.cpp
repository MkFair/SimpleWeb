#include "Handlers.h"

void RequestHandler::set_headers(std::map<std::string,std::string> headers){
        headers_ = headers;
        std::smatch m;
        try{
            std::regex re_mask("[\?|&]([^=]+)=([^&]+)");
            std::string s = headers_["REQUEST_URI"];
            
            while(std::regex_search(s,m,re_mask,std::regex_constants::match_any)){
                if( 3==m.size() ){
                    get_params[m[1].str()] = m[2].str();
                    s = m.suffix();
                }
             }
        }catch(std::exception& e){
            std::cout<<e.what();
        }
    }
std::string StaticHandler::handler(){

		if( std::filesystem::exists(std::string("static")+headers_["REQUEST_URN"]) and std::filesystem::is_regular_file(std::string("static")+headers_["REQUEST_URN"]) ){
			std::ifstream file(std::string("static")+headers_["REQUEST_URN"]);
			if(file.good()){
				std::string res((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
				file.close();
				std::string type("text/html");
				std::map<std::string,std::string> types;
				types["html"] = "text/html";
				types["jpg"] = "image/jpg";
				types["png"] = "image/png";
				types["svg"] = "image/svg";
				types["css"] = "text/css";
				types["js"] = "text/javascript";
				size_t pos_dot = headers_["REQUEST_URN"].find_last_of(".");
				if( pos_dot!= std::string::npos ){
					std::string subtype = headers_["REQUEST_URN"].substr(pos_dot+1);
					for(auto el: types){
						if(subtype == el.first){
							type = el.second;
						}
					}
				}
				std::string resp = "HTTP/1.1 200 OK\r\nContent-Type: ";
				resp += type;
				resp += "\r\nKeep-Alive: Close\r\n\r\n";
				resp += res;
				return resp;
			}else{
				throw FileNotFound();
			}
		}
		throw FileNotFound();
	}
std::string TemplateHandler::handler(){
    if( std::filesystem::exists(std::string("static")+filename_) and std::filesystem::is_regular_file(std::string("static")+filename_) ){
			std::ifstream file(std::string("static")+filename_);
			if(file.good()){
                std::string res((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
                std::string resp = "HTTP/1.1 200 OK\r\nContent-Type: text/html";
				
				resp += "\r\nKeep-Alive: Close\r\n\r\n";
				resp += res;
				return resp;
            }
    }
}
