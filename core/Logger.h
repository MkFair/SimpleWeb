#include <iostream>
#include <string>
#include <vector>


class Logger{
    bool console_enable_;
    std::vector<std::string> msgs;
    public:
    Logger(bool);
    void push(std::string);
    ~Logger(){};
};
