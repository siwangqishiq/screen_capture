
#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "purple.h"

class IScreenApi;
class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;
    std::shared_ptr<purple::TextureImage> image;
public:
    void appInit();

    void execute();

    void exit();

    void init();
    void tick();
};