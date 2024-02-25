
#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "purple.h"

class IScreenApi;

enum ScreenState{
    Idle
};

class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;
    std::shared_ptr<purple::TextureImage> image;
    std::shared_ptr<purple::TextureImage> mScreenImage;

    uint8_t* mScreenImagePixel = nullptr;
    int mScreenWidth = 0;
    int mScreenHeight = 0;

public:
    void appInit();

    void execute();

    void exit();

    void init();
    void tick();

    ScreenState mState = ScreenState::Idle;
};