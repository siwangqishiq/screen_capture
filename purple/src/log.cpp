#include "log.h"

namespace purple{
    const std::string Log::TAG = "log";
    const std::string Log::TIME_FORMAT = "%H:%M:%S";

    using SystemClk = std::chrono::system_clock;

    void Log::e(const std::string &tag , std::string msg){
        printf(LIGHT_RED"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }

    void Log::w(const std::string &tag , std::string msg){
        printf(YELLOW"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }

    void Log::i(const std::string &tag , std::string msg){
        printf(WHITE"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }

    std::string Log::currentShowTime(){
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto str = std::put_time(std::localtime(&t), "%X");
        std::stringstream ss;
        ss << str;
        return ss.str();
    }
}