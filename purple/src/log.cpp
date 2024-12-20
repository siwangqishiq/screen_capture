#include "log.h"

namespace purple{
    const std::string Log::TAG = "log";
    const std::string Log::TIME_FORMAT = "%H:%M:%S";

    using SystemClk = std::chrono::system_clock;
    
    extern bool UNITTEST;

#ifdef __ANDROID__
    #include <android/log.h>

    #define  LOG_TAG    "purple"
    #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, __VA_ARGS__)
    #define  LOGW(...)  __android_log_print(purple::ANDROID_LOG_WARN, __VA_ARGS__)
    #define  LOGE(...)  __android_log_print(purple::ANDROID_LOG_ERROR, __VA_ARGS__)

    void Log::e(const std::string &tag , std::string msg){
        LOGE(tag.c_str() , "%s" ,msg.c_str());
    }

    void Log::w(const std::string &tag , std::string msg){
        LOGW(tag.c_str() , "%s" ,msg.c_str());
    }

    void Log::i(const std::string &tag , std::string msg){
        LOGI(tag.c_str() , "%s" ,msg.c_str());
    }

#else
    void Log::e(const std::string &tag , std::string msg){
        if(UNITTEST){
            return;
        }

        printf(LIGHT_RED"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }

    void Log::w(const std::string &tag , std::string msg){
        if(UNITTEST){
            return;
        }
        printf(YELLOW"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }

    void Log::i(const std::string &tag , std::string msg){
        if(UNITTEST){
            return;
        }
        printf(WHITE"%s[%s]:%s\n" NONE ,currentShowTime().c_str(), tag.c_str() , msg.c_str());
    }
#endif

    std::string Log::currentShowTime(){
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto str = std::put_time(std::localtime(&t), "%X");
        std::stringstream ss;
        ss << str;
        return ss.str();
    }
}