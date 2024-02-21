
#include <iostream>
#include <Windows.h>

void findScreenSize(){
    // HDC screen = GetDC(nullptr);
    // int screenWidth = GetDeviceCaps(screen, HORZRES);
    // int screenHeight = GetDeviceCaps(screen, VERTRES);

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    std::cout << "screenWidth = " << screenWidth << std::endl;
    std::cout << "screenHeight = " << screenHeight << std::endl;
}

int main(){
    // std::cout << "Hello it works" << std::endl;
    findScreenSize();
    return 0;
}//end main
