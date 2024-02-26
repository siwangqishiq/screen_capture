
#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "purple.h"

class IScreenApi;

enum ScreenState{
    Idle,
    DRAW_CAPTURE_ZONE,// 抓取需要截取的区域
};

enum EventAction{
    ActionDown,
    ActionMove,
    ActionUp
};

class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;
    std::shared_ptr<purple::TextureImage> image;
    std::shared_ptr<purple::TextureImage> mScreenImage;

    uint8_t* mScreenImagePixel = nullptr;

    float mCaptureStartX = 0.0f;
    float mCaptureStartY = 0.0f;
    float mCaptureEndX = 0.0f;
    float mCaptureEndY = 0.0f;

    void renderScreenCaptureImage();
    void renderMaskZone();
public:
    void appInit();

    void execute();

    void exit();

    void init();
    void tick();

    void onResize(int w,int h);

    void onEventAction(EventAction action , float x , float y);

    ScreenState mState = ScreenState::Idle;

    bool mMouseActionDown = false;
    float mEventX = 0.0f;
    float mEventY = 0.0f;

    int mScreenWidth = 0;
    int mScreenHeight = 0;

    purple::Paint mMaskZonePaint;
};