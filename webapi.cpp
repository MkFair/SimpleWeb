#include <iostream>
#include "core/Server.h"
#include "core/Handlers.h"
class ApiIndex : public ApiHandler{
public:
	ApiIndex() : ApiHandler(){}
	virtual Json::Value handler() override {
		Json::Value jsonv;
		return jsonv;
	}
	virtual ~ApiIndex(){};
};
int main() {
    std::map<std::string,RequestHandler*> routings;
    routings["/"] = new ApiIndex();

	asio::io_context io_c;
	Server s(io_c,routings);
	io_c.run();

	return 0;
}
