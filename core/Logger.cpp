#include "Logger.h"

Logger::Logger(bool console_enable):
    console_enable_(console_enable)
{
}
void Logger::push(std::string msg){
    msgs.push_back(msg);
    if(console_enable_){
        std::cout<<msg<<std::endl<<std::flush;
    }
    
}
