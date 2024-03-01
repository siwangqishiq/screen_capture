
#include "application.h"
#include "bridge.h"
#include "purple.h"
#include "constants.h"
#include "menu.h"

void Application::appInit(){
    mScreenApi = std::make_shared<ScreenApi>(this);
}

void Application::exit(){
    if(window != nullptr){
        glfwSetWindowShouldClose(window, true);
    }
}


void Application::execute(){
    glfwInit();
        
    appInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED , GLFW_FALSE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    //窗口透明
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER , GLFW_TRUE);
    
    mScreenApi->findScreenSize(mScreenWidth , mScreenHeight);
    mScreenImagePixel = mScreenApi->captureScreen();

    #ifdef _WIN32
    window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", 
        nullptr, nullptr);
    #elif __linux__
    window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", 
        glfwGetPrimaryMonitor(), nullptr);
    #endif
    // window = glfwCreateWindow(400, 200, "screen capture", nullptr, nullptr);
    
    if (window == nullptr) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window , this);

    glfwSetWindowPos(window , 0 , 0);
    // glfwSetWindowAttrib(window , GLFW_DECORATED , GLFW_TRUE);
    glfwSetWindowAttrib(window , GLFW_DECORATED , GLFW_FALSE);

    glfwSetKeyCallback(window , [](GLFWwindow* windows_,int key,int scancode,int action,int mods){
        if(glfwGetKey(windows_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(windows_, true);
        }
    });
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* windows_,int w,int h){
        Application* app_ = static_cast<Application *>(glfwGetWindowUserPointer(windows_));
        app_->onResize(w , h);
    });

    glfwSetMouseButtonCallback(window , [](GLFWwindow* windows_,int button,int event,int mods){
        Application* app_ = static_cast<Application *>(glfwGetWindowUserPointer(windows_));
        if(button == GLFW_MOUSE_BUTTON_LEFT){ //左键事件
            if(event == GLFW_PRESS){
                app_->mMouseActionDown = true;
                app_->onEventAction(ActionDown , app_->mEventX 
                    , app_->mScreenHeight - app_->mEventY);
            }else if(event == GLFW_RELEASE){
                app_->mMouseActionDown = false;
                app_->onEventAction(ActionUp , app_->mEventX 
                    , app_->mScreenHeight - app_->mEventY);
            }
        }
        // std::cout << "event " << button << "  " << event << std::endl;
    });

    glfwSetCursorPosCallback(window , [](GLFWwindow* windows_,double x,double y){
        Application* app_ = static_cast<Application *>(glfwGetWindowUserPointer(windows_));
        app_->mEventX = x;
        app_->mEventY = y;
        if(app_->mMouseActionDown){
            app_->onEventAction(ActionMove , app_->mEventX 
                , app_->mScreenHeight - app_->mEventY);
        }
        // std::cout << "pos: " << mouseX << "  " << mouseY << std::endl;
    });

    // glfwSetCharCallback(windows , [](GLFWwindow* windows_ , unsigned int codepoint){
    // });
    
    purple::Engine::init(mScreenWidth , mScreenHeight);
    init();

    while(!glfwWindowShouldClose(window)) {
        tick();
        glfwSwapBuffers(window);
        glfwPollEvents();

        glfwSwapInterval(1);//锁定固定帧率
    }//end while
    
    mActionMenu->dispose();
    purple::Engine::dispose();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::init(){
    glEnable(GL_MULTISAMPLE);
    
    purple::Log::i("purple_engine" , "init");

    // image = purple::BuildImageByAsset(std::string("t2.jpg"));
    mMaskZonePaint.color = glm::vec4(0.0f , 0.0f ,0.0f , 0.45f);

    mMaskZoneBorderPaint.color = glm::vec4(0.0f , 1.0f ,0.0f , 1.0f);
    mMaskZoneBorderPaint.fillStyle = purple::FillStyle::Stroken; 

    mScreenImage = purple::BuildImageByPixlData(mScreenImagePixel , mScreenWidth , mScreenHeight , GL_RGB);

    mActionMenu = std::make_shared<ActionMenu>(this);
    mActionMenu->init();
}

void Application::tick(){
    purple::Engine::tick();
    mActionMenu->update();

    render();
}

void Application::render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw bottom image (screen image)
    renderScreenCaptureImage();

    renderMaskZone();
    renderSubThumbPreview();

    //menu render
    mActionMenu->render();
}

void Application::renderScreenCaptureImage(){
    purple::Rect imgDstRect;
    imgDstRect.left = 0.0f;
    imgDstRect.top = purple::Engine::ScreenHeight;
    imgDstRect.width = purple::Engine::ScreenWidth;
    imgDstRect.height = purple::Engine::ScreenHeight;
    auto spriteBatch  = purple::Engine::getRenderEngine()->getSpriteBatch();

    spriteBatch->begin();
    auto src = mScreenImage->getRect();
    spriteBatch->renderImage(*mScreenImage , src , imgDstRect);
    spriteBatch->end();
}

std::vector<float> Application::calClipPoints(){
    std::vector<float> result(4);
    result[0] = mCaptureStartX < mCaptureEndX?mCaptureStartX:mCaptureEndX;//left
    result[1] = mCaptureStartX < mCaptureEndX?mCaptureEndX:mCaptureStartX;//right
    result[2] = mCaptureStartY > mCaptureEndY?mCaptureStartY:mCaptureEndY;//top
    result[3] = mCaptureStartY > mCaptureEndY?mCaptureEndY:mCaptureStartY;//bottom
    return result;
}

void Application::renderMaskZone(){
    if(isReadPixelMode){ 
        return;
    }

    auto result = calClipPoints();

    float left = result[0];
    float right = result[1];
    float top = result[2];
    float bottom = result[3];

    if(mState == ScreenState::Idle){
        left = 0.0f;
        right = 0.0f;
        top = mScreenHeight;
        bottom = mScreenHeight;
    }
    // purple::Log::i("left" , "left = %f mCaptureEndX = %f" ,left ,mCaptureEndX);

    purple::Rect topRect;
    topRect.left = 0.0f;
    topRect.top = mScreenHeight;
    topRect.width = mScreenWidth;
    topRect.height = mScreenHeight - top;

    purple::Rect bottomRect;
    bottomRect.left = 0.0f;
    bottomRect.top = bottom;
    bottomRect.width = mScreenWidth;
    bottomRect.height = bottom;
    // purple::Log::i("bottomRect" , "bottomRect %f,%f,%f,%f" , bottomRect.left,
    //     bottomRect.top , bottomRect.width , bottomRect.height);

    purple::Rect leftRect;
    leftRect.left = 0.0f;
    leftRect.top = top;
    leftRect.width = left;
    leftRect.height = top - bottom;

    purple::Rect rightRect;
    rightRect.left = right;
    rightRect.top = top;
    rightRect.width = mScreenWidth - right;
    rightRect.height = top - bottom;

    purple::Rect zoneBorderRect;
    zoneBorderRect.left = left;
    zoneBorderRect.top = top;
    zoneBorderRect.width = right - left;
    zoneBorderRect.height = top - bottom;

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(topRect , mMaskZonePaint);
    shapeBatch->renderRect(bottomRect , mMaskZonePaint);
    shapeBatch->renderRect(leftRect , mMaskZonePaint);
    shapeBatch->renderRect(rightRect , mMaskZonePaint);
    shapeBatch->renderRect(zoneBorderRect , mMaskZoneBorderPaint);
    shapeBatch->end();

    const int clipWidth = static_cast<int>(zoneBorderRect.width);
    const int clipHeight = static_cast<int>(zoneBorderRect.height);
    std::wstring clipSizeStr = std::to_wstring(clipWidth)+L" x " 
        + std::to_wstring(clipHeight);
    purple::TextPaint txtPaint;
    const float textSize = 20.0f;
    const float padding = 4.0f;
    txtPaint.setTextSize(textSize);

    if(top + textSize + padding >= mScreenHeight){
        txtPaint.textColor = App::COLOR_BLACK;
        purple::Engine::getRenderEngine()->renderText(clipSizeStr , 
            left + padding , top - textSize - padding , txtPaint);
    }else{
        txtPaint.textColor = App::COLOR_WHITE;
        purple::Engine::getRenderEngine()->renderText(clipSizeStr , 
            left + padding , top + padding , txtPaint);
    }
}

// 细节预览窗口渲染
void Application::renderSubThumbPreview(){
    if(mState != DRAW_CAPTURE_ZONE){
        return;
    }

    purple::Rect srcRect;
    srcRect.left = mEventX - mThumbPreviewSize / 2.0f;
    srcRect.top = mScreenHeight - (mEventY - mThumbPreviewSize / 2.0f);
    srcRect.width = mThumbPreviewSize;
    srcRect.height = mThumbPreviewSize;

    const float previewOffsetX = 24.0f;
    const float previewOffsetY = 32.0f;
    float previewLeft = mEventX + previewOffsetX;
    float previewTop = mScreenHeight - mEventY - previewOffsetY;

    purple::Rect dstRect;
    dstRect.left = previewLeft;
    dstRect.top = previewTop;
    dstRect.width = srcRect.width * mScaleThumbFactor;
    dstRect.height = srcRect.height * mScaleThumbFactor;

    const float infoTextSize = 20.0f;
    purple::Rect infoRect = dstRect;
    infoRect.top = dstRect.getBottom() - 8.0f;
    infoRect.width = 200.0f;
    infoRect.height = infoTextSize * 2.0f + 16.0f;
    purple::TextPaint txtPaint;
    txtPaint.setTextSize(infoTextSize);
    txtPaint.textGravity = purple::TextGravity::CenterLeft; 

    //调整预览窗口位置
    adjustScalePreviewWinPosition(previewLeft , previewTop , dstRect.width , dstRect.height + infoRect.height);

    //reset postion
    dstRect.left = previewLeft;
    dstRect.top = previewTop;
    infoRect.left = dstRect.left;
    infoRect.top = dstRect.getBottom() - 8.0f;

    auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
    spriteBatch->begin();
    spriteBatch->renderImage(*mScreenImage, srcRect , dstRect);
    spriteBatch->end();  

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(dstRect , mMaskZoneBorderPaint);
    shapeBatch->end();

    std::vector<float> lines(8);
    lines[0] = dstRect.left;
    lines[1] = dstRect.top - dstRect.height / 2.0f;
    lines[2] = dstRect.getRight();
    lines[3] = dstRect.top - dstRect.height / 2.0f;
    lines[4] = dstRect.left + dstRect.width / 2.0f;
    lines[5] = dstRect.top;
    lines[6] = dstRect.left + dstRect.width / 2.0f;
    lines[7] = dstRect.getBottom();
    purple::Paint linesPaint;
    linesPaint.color = App::COLOR_GREEN;
    linesPaint.stokenWidth = 1.0f;
    purple::Engine::getRenderEngine()->renderLines(lines , linesPaint);

    const int posX = static_cast<int>(mEventX);
    const int posY = static_cast<int>(mEventY);
    std::wstring infoStr = 
        L" 位置(" + std::to_wstring(posX) + 
        L" , " + std::to_wstring(posY) + L")";

    if(mScreenImagePixel != nullptr){
        int offset = (mScreenHeight - posY) * mScreenWidth + posX;
        offset = (offset << 1) + offset;
        const int pixelSizeLimit = mScreenWidth * mScreenHeight * 3;
        if(offset >= 0 && offset < pixelSizeLimit ){
            uint8_t colorR = mScreenImagePixel[offset + 0];
            uint8_t colorG = mScreenImagePixel[offset + 1];
            uint8_t colorB = mScreenImagePixel[offset + 2];

            infoStr += L"\n 颜色(" + std::to_wstring(colorR) 
                + L"," + std::to_wstring(colorG)
                + L"," + std::to_wstring(colorB) 
                + L")";
        }
    }
    
    purple::Rect warpRect;
    purple::Engine::getRenderEngine()->renderTextWithRect(infoStr , 
        infoRect , txtPaint , &warpRect);
    shapeBatch->begin();
    purple::Paint txtWrapPaint;
    txtWrapPaint.color = App::COLOR_BLACK;
    if(infoRect.width < warpRect.width){
        infoRect.width = warpRect.width;
    }
    shapeBatch->renderRect(infoRect , txtWrapPaint);
    shapeBatch->end();
    purple::Engine::getRenderEngine()->renderTextWithRect(infoStr , 
        infoRect , txtPaint , nullptr);
}

void Application::adjustScalePreviewWinPosition(float &originLeft , float &originTop , 
        float preWinWidth , float preWinHeight){
    if(originTop - preWinHeight < 0){
        originTop = preWinHeight;
    } 

    if(originLeft + preWinWidth > mScreenWidth){
        originLeft = mScreenWidth - preWinWidth;
    }
}

void Application::onResize(int w , int h){
    purple::Log::i("resize" , "width = %d , height = %d" , w , h);
}

void Application::onEventAction(EventAction action , float x , float y){
    // mEventX = x;
    // mEventY = y;

    if(mActionMenu->dispatchEventAction(action , x , y)){
        return;
    }

    if(mState == Idle || mState == CAPTURE_ZONE_GETTED){ // idle空闲状态
        mState = DRAW_CAPTURE_ZONE;
        switch (action){
        case ActionDown:
            mCaptureStartX = x;
            mCaptureStartY = y;
            mCaptureEndX = x;
            mCaptureEndY = y;
            break;
        default:
            break;
        }//end switch
    }else if(mState == DRAW_CAPTURE_ZONE){
        // purple::Log::i("onEventAction" , "x = %f , y = %f" , x , y);
        switch (action){
        case ActionDown:
            mCaptureStartX = x;
            mCaptureStartY = y;
            break;
        case ActionMove:
            mCaptureEndX = x;
            mCaptureEndY = y;
            break;
        case ActionUp:
            mCaptureEndX = x;
            mCaptureEndY = y;
            purple::Log::i("onEventAction" , "x = %f , y = %f" , x , y);
            std::vector<float> ps = calClipPoints();
            const int width = static_cast<int>(ps[1] - ps[0]);
            const int height = static_cast<int>(ps[2] - ps[3]);
            mState = (width <= 0 || height <= 0)?Idle:CAPTURE_ZONE_GETTED;
            break;
        }//end switch
    }//end if

    // purple::Log::i("onEventAction" , "action = %d ,(%f , %f)" , action , x , y);
}