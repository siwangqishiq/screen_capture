
#pragma once

#include <memory>
#include "GLFW/glfw3.h"

class IScreenApi;
class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;
public:
    void appInit();

    void execute();

    void exit();
};