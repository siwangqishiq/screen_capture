#include "bridge.h"

//for windows
#ifdef _WIN32
#include <Windows.h>

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

// for linux
#elif __linux__

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

// for mac
#elif __APPLE__

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

#else

#endif
