
#include <iostream>
#include <thread>
#include <memory>
#include "application.h"
#include "bridge.h"
#include <string>

void subThreadEntry(){
    Application app;
    app.execute();
}

int main(){
    std::thread th(subThreadEntry);
    std::cout << "wait th thread ended..." << std::endl;
    th.join();
    std::cout << "main thread finished" << std::endl;
    return 0;
}//end main
