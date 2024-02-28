
#include <iostream>
#include <thread>
#include <memory>
#include "application.h"
#include "bridge.h"

void subThreadEntry(){
    Application app;
    app.execute();
}

int main(){
    // std::cout << "Hello it works" << std::endl;
    // findScreenSize();

    std::thread th(subThreadEntry);

    std::cout << "wait th thread ended..." << std::endl;
    th.join();
    std::cout << "main thread finished" << std::endl;
    return 0;
}//end main
