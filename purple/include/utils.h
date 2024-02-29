#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace purple{
    inline std::string date_time(std::time_t posix)
    {
        char buf[20]; // big enough for 2015-07-08 10:06:51\0
        std::tm tp = *std::localtime(&posix);
        return {buf, std::strftime(buf, sizeof(buf), "%F %T", &tp)};
    }

    inline long long currentTimeMillis(){
        using namespace std;
        using namespace std::chrono;

        // get absolute wall time
        auto now = system_clock::now();

        // find the number of milliseconds
        auto ms = duration_cast<milliseconds>(now.time_since_epoch());
        return ms.count();
    }

    inline long long currentTimeMicro(){
        using namespace std;
        using namespace std::chrono;

        auto now = system_clock::now();
        
        auto us = duration_cast<microseconds>(now.time_since_epoch());

        return us.count();
    }

    
}


