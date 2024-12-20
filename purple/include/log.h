#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdarg>

namespace purple{
    #define _CRT_SECURE_NO_WARNINGS 

    template<typename... Args>
    std::string FormatString(std::string format , Args... args){
        int lenStr = std::snprintf(nullptr, 0, format.c_str(), args...);
        if(lenStr <= 0){
            return "";
        }

        lenStr++;
        auto pstrOut =new char[lenStr];
        std::snprintf(pstrOut, lenStr, format.c_str(), args...);
        std::string str(pstrOut);

        delete[] pstrOut;
        return str;
    }

    // inline std::wstring FormatWstring(const wchar_t * _Format, ...){
    //     std::wstring _str;

    //     std::wstring tmp;
    //     va_list marker = NULL;
    //     va_start(marker, _Format);
    //     size_t num_of_chars = _vscwprintf(_Format, marker);
    //     if (num_of_chars > tmp.capacity()) {
    //         tmp.resize(num_of_chars + 1);
    //     }

    //     vswprintf_s((wchar_t *)tmp.data(), tmp.capacity(), _Format, marker);
    //     va_end(marker);
    //     _str = tmp.c_str();
    //     return _str;
    // }

    /**
     * @brief 
     * log utils
     *  string format
     * 
     */
    class Log{
        public:
            const static std::string TAG;
            const static std::string TIME_FORMAT;

            #define NONE         "\033[m"
            #define RED          "\033[0;32;31m"
            #define LIGHT_RED    "\033[1;31m"
            #define GREEN        "\033[0;32;32m"
            #define LIGHT_GREEN  "\033[1;32m"
            #define BLUE         "\033[0;32;34m"
            #define LIGHT_BLUE   "\033[1;34m"
            #define DARY_GRAY    "\033[1;30m"
            #define CYAN         "\033[0;36m"
            #define LIGHT_CYAN   "\033[1;36m"
            #define PURPLE       "\033[0;35m"
            #define LIGHT_PURPLE "\033[1;35m"
            #define BROWN        "\033[0;33m"
            #define YELLOW       "\033[1;33m"
            #define LIGHT_GRAY   "\033[0;37m"
            #define WHITE        "\033[1;37m"
            
            static void e(const std::string &tag , std::string msg);
            static void w(const std::string &tag , std::string msg);
            static void i(const std::string &tag , std::string msg);

            static void log(std::string msg){
                i(TAG ,msg);
            }

            template<typename... Args>
            static void e(const std::string &tag,const std::string format , Args... args){
                e(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void i(const std::string &tag,const std::string format , Args... args){
                i(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void w(const std::string &tag,const std::string format , Args... args){
                w(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void log(const std::string format , Args... args){
                log(FormatString(format , args...));
            }
            
            static std::string currentShowTime();
    };//end class Log
}