
#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "purple.h"
#include <vector>
#include "common.h"

class IScreenApi;
class ActionMenu;
class IEditor;
class TextureInfo;
class TextEditor;

enum ScreenState{
    Idle,
    DRAW_CAPTURE_ZONE,// 抓取需要截取的区域
    RESIZE_CAPTURE_ZONE,//截取区域调整
    MOVE_CAPTURE_ZONE,//移动截取区域
    CAPTURE_ZONE_GETTED,//取得截取区域
    CAPTURE_INSERT_TEXT,//截取区域 插入文本
    CAPTURE_ZONE_EDIT //截取区域编辑
};



enum ResizeType{
    None = 0,
    LeftTop = 1,
    HmidTop = 2,
    RightTop = 3,
    RightVmid = 4,
    RightBottom = 5,
    HmidBottom = 6,
    LeftBottom = 7,
    LeftVmid = 8
};

enum CursorType{
    Normal = 0,
    Cross = 1,
    Move = 2
};

class Application{
private:
    GLFWwindow *window;
    std::shared_ptr<IScreenApi> mScreenApi;

    uint8_t* mScreenImagePixel = nullptr;

    float mCaptureStartX = 0.0f;
    float mCaptureStartY = 0.0f;
    float mCaptureEndX = 0.0f;
    float mCaptureEndY = 0.0f;


    void renderScreenCaptureImage();
    void renderEditorContent();
    void renderMaskZone();
    void renderSubThumbPreview();

    void adjustScalePreviewWinPosition(float &originLeft , float &originTop , 
            float preWinWidth , float preWinHeight);

    void moveEditorToList(std::shared_ptr<IEditor> editor);
    
    int fps = 0;
    int renderTimes = 0;

    ResizeType mResizeType = ResizeType::None;
    float mLastX;
    float mLastY;

    std::shared_ptr<purple::TextureInfo> mVirtualTexture;

    CursorType mCursorType = CursorType::Normal;
public:
    static bool isDebug;
    static bool isWindows;
    static bool isMac;
    static bool isLinux;

    bool mAntiAliasing = true; //是否开启反锯齿

    std::wstring mInputContent;

    std::string mOutputFilePath = "test_screen.png";

    double mLastClickTime = 0.0;

    bool isReadPixelMode = false;
    
    void appInit();

    void execute();

    void exit();

    void init();
    void tick();
    void dispose();
    void render();

    void updateControllButtons();

    void renderControllButtons();

    void onResize(int w,int h);

    void onEventAction(EventAction action , float x , float y);

    //设置当前编辑器
    bool setCurrentEditor(std::shared_ptr<IEditor> editor);

    //是否可以重置选区
    bool canResetClipZone();

    //鼠标双击
    bool onMouseDoubleClick();

    void resizeUpdate(EventAction action , float x , float y);

    void moveCaptureZone(EventAction action , float x , float y);

    void adjustMoveCaptureZone(float dx , float dy);

    void updateCursor(CursorType newCursorType);

    void onInputContentChange(std::wstring newContent);

    void onDeleteKeyPressed();

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

    std::shared_ptr<purple::TextureImage> mScreenImage;

    purple::Paint mMaskZonePaint;
    purple::Paint mMaskZoneBorderPaint;

    std::shared_ptr<ActionMenu> mActionMenu; //操作菜单

    std::vector<std::shared_ptr<IEditor>> mEditorList; //编辑操作列表

    std::shared_ptr<IEditor> mCurrentEditor = nullptr;
    
    std::vector<purple::Rect> mControlButtonRects; //resize control buttonsrect

    //cursor
    GLFWcursor *mCrossCursor = nullptr;
    GLFWcursor *mMoveCursor = nullptr;
    
    std::shared_ptr<ITouch> mGrapTouchEntity = nullptr;

    GLFWcursor* loadCursorResFromFile(std::string path);

    std::shared_ptr<TextEditor> findActionPointInTextEditor(float x , float y);
};