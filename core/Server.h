#include <iostream>
#include <asio.hpp>
#include <regex>
#include <iterator>
#include <cstring>
#include "Handlers.h"
#include "Logger.h"
#include "Headers.h"


class ClientHandler{
    asio::ip::tcp::socket s_;
    std::string buf_;
    int data_len_;
    std::map<std::string,RequestHandler*> routings_;
    public:
    ClientHandler(asio::ip::tcp::socket,std::map<std::string,RequestHandler*>);
    virtual ~ClientHandler(){};
    void handler();
};

class Server{
	asio::ip::tcp::acceptor acceptor_;
    std::map<std::string,RequestHandler*> routings_;
    Logger logger_;
public:
	Server(asio::io_context & io_context,uint32_t port, std::map<std::string,RequestHandler*>routings);
	void accept();
	virtual ~Server(){};

};
