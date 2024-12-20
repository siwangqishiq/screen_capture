#pragma once

#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "render/common.h"

namespace purple{
    
    std::string date_time(std::time_t posix);

    long long currentTimeMillis();

    long long currentTimeMicro();

    void ScaleWithPoint(Point &point , float scale, Point center);

    void Rotate(Point &point, float cx , float cy , float angle);
}


