
#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "purple.h"
#include <vector>

class IScreenApi;
class ActionMenu;

enum ScreenState{
    Idle,
    DRAW_CAPTURE_ZONE,// 抓取需要截取的区域
    CAPTURE_ZONE_GETTED//取得截取区域
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
    void renderSubThumbPreview();

    void adjustScalePreviewWinPosition(float &originLeft , float &originTop , 
            float preWinWidth , float preWinHeight);
    
public:
    static bool isWindows;
    static bool isMac;
    static bool isLinux;

    bool isReadPixelMode = false;
    
    void appInit();

    void execute();

    void exit();

    void init();
    void tick();
    void render();

    void onResize(int w,int h);

    void onEventAction(EventAction action , float x , float y);

    //计算裁剪点坐标 
    std::vector<float> calClipPoints();

    ScreenState mState = ScreenState::Idle;

    bool mMouseActionDown = false;
    float mEventX = 0.0f;
    float mEventY = 0.0f;

    int mScreenWidth = 0;
    int mScreenHeight = 0;

    float mScaleThumbFactor = 3.0f;//子略缩图 放大倍数
    float mThumbPreviewSize = 64.0f;

    purple::Paint mMaskZonePaint;
    purple::Paint mMaskZoneBorderPaint;

    //操作菜单
    std::shared_ptr<ActionMenu> mActionMenu;
};