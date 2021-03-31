#include <iostream>
#include <asio.hpp>
#include <regex>
#include <iterator>
#include <cstring>
#include "Handlers.h"
#include "Logger.h"
#include "Headers.h"

class Server{
	asio::ip::tcp::acceptor acceptor_;
    std::map<std::string,RequestHandler*> routings_;
    Logger logger_;
public:
	Server(asio::io_context & io_context,std::map<std::string,RequestHandler*>routings);
	void accept();
	virtual ~Server(){};

};
