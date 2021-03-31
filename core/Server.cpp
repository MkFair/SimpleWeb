#include "Server.h"
Server::Server(asio::io_context & io_context,std::map<std::string,RequestHandler*>routings):
    acceptor_(io_context,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),45088)),
    logger_(true)
{
        routings_ = routings;
		asio::socket_base::reuse_address opt(true);
		acceptor_.set_option(opt);
		accept();
}


void Server::accept(){
		acceptor_.async_accept([this](asio::error_code ec, asio::ip::tcp::socket s){
            
            logger_.push(std::string("Accept new request from ")+s.remote_endpoint().address().to_string());
			if(!ec){
				std::string buf;
				buf.resize(4096);
				int data_len = 0;
				asio::error_code ec_recv;

				data_len = s.read_some(asio::buffer(buf,4096),ec_recv);
				if(data_len and !ec_recv){
					ulong pos =  buf.find("\r\n\r\n");
					if( pos!=std::string::npos and pos<buf.size()){//если есть обязательный двойной перенос по http обрабатываем
						Headers hh(buf.substr(0,pos));
                        std::map<std::string,std::string> headers = hh.get_headers();
						headers["CLIENT_IP"] = s.remote_endpoint().address().to_string();
                        std::string resp;
						//обработка запроса
						try{
							StaticHandler sth = StaticHandler();
                            sth.set_headers(headers);
							resp = sth.build_resp();
						}
						catch(FileNotFound& e){//если не смог обработать
                            std::map<std::string,std::string> partype;
                            partype["<int>"] = "(\\d+)";
                            partype["<str>"] = "([^/]+)";
                            partype["<float>"] = "(\\d+\\.\\d+)";
                            for(auto el : routings_){
                                std::string mask_url = el.first;
                                bool state = false;
                                
                                for(auto tel:partype){
                                    ulong pos_re = mask_url.find(tel.first);
                                    if(pos_re != std::string::npos and pos_re<mask_url.size()){
                                        mask_url.replace(pos_re,tel.first.size(),tel.second);
                                        std::regex re_mask(mask_url) ; 
                                        state = std::regex_match(headers["REQUEST_URN"],re_mask);
                                        
                                    }
                                }
                                //std::cout<<mask_url;
                                if(headers["REQUEST_URN"]==el.first or state){
                                    
                                    try{
                                        ApiHandler * aph = static_cast<ApiHandler*>(el.second);
                                        if(state){
                                            std::smatch match;
                                            
                                            if(std::regex_search(headers["REQUEST_URN"],match,std::regex(mask_url))){
                                                std::cout<<"search ok"<<match.size();
                                                if(match.size()>0){
                                                    for(uint i=1; i<match.size(); i++){
                                                        aph->params_by_key[i-1] = match[i];
                                                    }
                                                    std::cout<<aph->params_by_key[0];
                                                }
                                            }
                                        }
                                        aph->set_headers(headers);
                                        resp = aph->build_resp();
                                    }
                                    catch(std::exception& e){
                                        std::cout<<"Error "<<e.what()<<std::endl;
                                    }
                                }
                            }
                            if(resp.empty())
								resp = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\nKeep-Alive: Close\r\n\r\n";
						}
						std::string senddata(resp);
						s.send(asio::buffer(senddata));
					}
				}else{
					std::cout<<ec_recv.message()<<"1"<<std::endl;
				}
				s.close();
			}
			accept();
		});
	}
