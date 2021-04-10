#include "Server.h"

ClientHandler::ClientHandler(asio::ip::tcp::socket s,std::map<std::string,RequestHandler*> routings)
    :s_(std::move(s)),
    routings_(routings),
    data_len_(0)
{
    buf_.resize(4096);
}
void ClientHandler::handler(){
    asio::error_code ec_recv;
    data_len_ = s_.read_some(asio::buffer(buf_,4096),ec_recv);
    if(data_len_ and !ec_recv){
		ulong pos =  buf_.find("\r\n\r\n");
        if( pos!=std::string::npos and pos<buf_.size()){//если есть обязательный двойной перенос по http обрабатываем
            Headers hh(buf_.substr(0,pos),s_.remote_endpoint().address().to_string());
            std::map<std::string,std::string> headers = hh.get_headers();
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
                if(resp.empty()){
                    ErrorHandler eh = ErrorHandler();
                    resp = eh.build_resp();
                }
            }
            std::string senddata(resp);
            s_.send(asio::buffer(senddata));
        }
    }else{
        std::cout<<ec_recv.message()<<"1"<<std::endl;
    }
    s_.close();
}
Server::Server(asio::io_context & io_context, uint32_t port, std::map<std::string,RequestHandler*>routings):
    acceptor_(io_context,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)),
    logger_(true)
{
    logger_.push(std::string("Starting server 0.0.0.0:")+std::to_string(port));
    routings_ = routings;
    asio::socket_base::reuse_address opt(true);
    acceptor_.set_option(opt);
    accept();
}


void Server::accept(){
		acceptor_.async_accept([this](asio::error_code ec, asio::ip::tcp::socket s){
            
            logger_.push(std::string("Accept new request from ")+s.remote_endpoint().address().to_string());
			if(!ec){
				ClientHandler ch = ClientHandler(std::move(s),routings_);
                ch.handler();
			}
			accept();
		});
	}
