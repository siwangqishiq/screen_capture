
#include "application.h"
#include "platform/bridge.h"
#include "purple.h"
#include "config/constants.h"
#include "action/menu.h"
#include "editor/editor.h"
#include "action/action_confirm.h"

bool Application::isDebug = true;

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

    // glfwWindowHint(GLFW_SAMPLES, 4);
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    //窗口透明
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER , GLFW_TRUE);
    
    mScreenApi->findScreenSize(mScreenWidth , mScreenHeight);
    mScreenImagePixel = mScreenApi->captureScreen();

    #ifdef _WIN32
    window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", 
        nullptr, nullptr);
    #elif __linux__
    //linux系统下 必须全屏 才能截取到全部的画面
    window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", 
        glfwGetPrimaryMonitor(), nullptr);
    #endif

    // window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", 
    //     glfwGetPrimaryMonitor(), nullptr);
    
    if (window == nullptr) {
        const char *desc;
        glfwGetError(&desc);
        std::cout << desc << std::endl;
        std::cerr << "window create failed!!!" << std::endl;
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
                bool isCost = false;
                double currentTime = glfwGetTime();
                if(currentTime - app_->mLastClickTime < 0.2f){
                    isCost = app_->onMouseDoubleClick();
                }
                app_->mLastClickTime = currentTime;
                app_->mMouseActionDown = true;

                if(!isCost){
                    app_->onEventAction(ActionDown , app_->mEventX 
                        , app_->mScreenHeight - app_->mEventY);
                }
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

    glfwSetCharCallback(window , [](GLFWwindow* windows_ , unsigned int codepoint){
        // Application* app_ = static_cast<Application *>(glfwGetWindowUserPointer(windows_));
        // // app_->mInputContent += std::to_wstring(codepoint);
        // wchar_t ch = codepoint;
        // app_->mInputContent += ch;                
    });

    // mMoveCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    // glfwSetCursor(window , mMoveCursor);
    
    //紫电引擎初始化
    purple::Engine::init(mScreenWidth , mScreenHeight);

    //应用初始化
    init();

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        tick();
        glfwSwapBuffers(window);
        glfwSwapInterval(1);//锁定固定帧率
    }//end while
    
    mActionMenu->dispose();
    purple::Engine::dispose();

    dispose();
}

void Application::dispose(){
    // std::cout << "Application::dispose..." << std::endl;
    if(mMoveCursor != nullptr){
        glfwDestroyCursor(mMoveCursor);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    // std::cout << "Application::dispose ended" << std::endl;
}

void Application::init(){
    // glEnable(GL_MULTISAMPLE);

    purple::Log::i("purple_engine" , "init");

    // image = purple::BuildImageByAsset(std::string("t2.jpg"));
    mMaskZonePaint.color = glm::vec4(0.0f , 0.0f ,0.0f , 0.45f);

    mMaskZoneBorderPaint.color = glm::vec4(0.0f , 1.0f ,0.0f , 1.0f);
    mMaskZoneBorderPaint.fillStyle = purple::FillStyle::Stroken; 

    mScreenImage = purple::BuildImageByPixlData(mScreenImagePixel , mScreenWidth , mScreenHeight , GL_RGB);
    
    mActionMenu = std::make_shared<ActionMenu>(this);
    mActionMenu->init();

    mControlButtonRects = std::vector<purple::Rect>(8);

    purple::Engine::getTimer()->scheduleAtFixedRate([this](void *app){
        fps = renderTimes;
        renderTimes = 0;
    } , 1000);

    mVirtualTexture = purple::Engine::getRenderEngine()->buildVirtualTexture("v_texture" , 200,400, 
        [&](int w, int h){
            purple::Log::w("VirtualTexture" , "VirtualTexture size = %d , %d" , w , h);
            
            purple::Rect rect1(0.0f , h , w, h);
            purple::Paint paint;
            paint.color = glm::vec4(0.0f , 1.0f , 0.0f ,1.0f);
            auto batch = purple::Engine::getRenderEngine()->getShapeBatch();
            batch->begin();
            batch->renderRoundRect(rect1 , 10.0f  , paint);
            batch->end();
        }
    );
}

void Application::tick(){
    purple::Engine::tick();

    updateControllButtons();
    mActionMenu->update();

    render();


    //for debug
    if(isDebug){
        // purple::Engine::getRenderEngine()->updateVirtualTexture(mVirtualTexture,
        // [](int w , int h){
        //     purple::Rect rect1(0.0f , h , w, h);
        //     purple::Paint paint;
        //     paint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);
        //     auto batch = purple::Engine::getRenderEngine()->getShapeBatch();
        //     batch->begin();
        //     batch->renderRoundRect(rect1 , 10.0f  , paint);
        //     batch->end();
        // });

        // auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
        // spriteBatch->begin();
        // purple::Rect srcRect(0.0f, mVirtualTexture->height / 1.0f
        //     , (float)(mVirtualTexture->width) , (float)(mVirtualTexture->height));
        // purple::TextureImage image(mVirtualTexture);
        // purple::Rect dst(0 , mScreenHeight , 100 , 100);
        // spriteBatch->renderImage(image , srcRect , dst);
        // spriteBatch->end();
        

        purple::TextPaint fpsPaint;
        fpsPaint.setTextSize(50.0f);

        purple::Engine::getRenderEngine()->renderText(mInputContent , 
            0.0f , mScreenHeight - 50.0f,
            fpsPaint);

        renderTimes++;
        std::wstring fpsStr = L"" + std::to_wstring(fps);
        purple::Engine::getRenderEngine()->renderText(fpsStr , 
            mScreenWidth - 100.0f , mScreenHeight - 50.0f,
            fpsPaint);
    }
}

void Application::updateControllButtons(){
    auto result = calClipPoints();
    const float left = result[0];
    const float right = result[1];
    const float top = result[2];
    const float bottom = result[3];

    if(right - left < 1.0f || top - bottom < 1.0f){
        return;
    }

    const float radius = 4.0f;
    const float size = 2.0f * radius;
    mControlButtonRects[0].left = left - radius;
    mControlButtonRects[0].top = top + radius;
    mControlButtonRects[0].width = size;
    mControlButtonRects[0].height = size;

    float midX = (left + right) / 2.0f;
    mControlButtonRects[1].left = midX - radius;
    mControlButtonRects[1].top = top + radius;
    mControlButtonRects[1].width = size;
    mControlButtonRects[1].height = size;

    mControlButtonRects[2].left = right - radius;
    mControlButtonRects[2].top = top + radius;
    mControlButtonRects[2].width = size;
    mControlButtonRects[2].height = size;

    float midY = (bottom + top) / 2.0f;
    mControlButtonRects[3].left = right - radius;
    mControlButtonRects[3].top = midY + radius;
    mControlButtonRects[3].width = size;
    mControlButtonRects[3].height = size;

    mControlButtonRects[4].left = right - radius;
    mControlButtonRects[4].top = bottom + radius;
    mControlButtonRects[4].width = size;
    mControlButtonRects[4].height = size;

    mControlButtonRects[5].left = midX - radius;
    mControlButtonRects[5].top = bottom + radius;
    mControlButtonRects[5].width = size;
    mControlButtonRects[5].height = size;
    
    mControlButtonRects[6].left = left - radius;
    mControlButtonRects[6].top = bottom + radius;
    mControlButtonRects[6].width = size;
    mControlButtonRects[6].height = size;

    mControlButtonRects[7].left = left - radius;
    mControlButtonRects[7].top = midY + radius;
    mControlButtonRects[7].width = size;
    mControlButtonRects[7].height = size;
}

void Application::renderControllButtons(){
    if(!canResetClipZone()){
        return;
    }

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    auto paint = mMaskZoneBorderPaint;
    paint.fillStyle = purple::FillStyle::Filled;

    shapeBatch->begin();
    for(purple::Rect &rect : mControlButtonRects){
        shapeBatch->renderRect(rect , paint);
    }//end for each
    shapeBatch->end();
}

void Application::render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderScreenCaptureImage();// draw bottom image (screen image)
    renderEditorContent();//editor content render

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

void Application::renderEditorContent(){
    for(std::shared_ptr<IEditor> pEd : mEditorList){
        pEd->renderEditorContent();
    }//end for each

    if(mCurrentEditor != nullptr){
        mCurrentEditor->renderEditorContent();
    }//end if
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

    //render control button
    renderControllButtons();
}

// 细节预览窗口渲染
void Application::renderSubThumbPreview(){
    if(mState != DRAW_CAPTURE_ZONE 
        && mState != RESIZE_CAPTURE_ZONE){
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

    const float infoTextSize = 24.0f;
    purple::Rect infoRect = dstRect;
    infoRect.top = dstRect.getBottom() - 8.0f;
    infoRect.width = 230.0f;
    infoRect.height = infoTextSize * 2.0f + 16.0f;
    purple::TextPaint txtPaint;
    txtPaint.setTextSize(infoTextSize);
    txtPaint.textGravity = purple::TextGravity::CenterLeft; 

    //调整预览窗口位置
    adjustScalePreviewWinPosition(previewLeft , previewTop , 
            dstRect.width , 
            dstRect.height + infoRect.height);

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

    if(mState == Idle){ // idle空闲状态
        if(!canResetClipZone()){ //已经确定选区的情况下 有编辑内容 这时不能重置选区
            return;
        }

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
    }else if(mState == CAPTURE_ZONE_EDIT){//编辑模式
        // purple::Log::i("onEventAction" , "action: %d , x = %f , y = %f" ,action, x , y);
        if(mCurrentEditor != nullptr){
            mCurrentEditor->dispatchEventAction(action , x , y);
        }
    }else if(mState == RESIZE_CAPTURE_ZONE){ //调整选区
        switch (action){
        case ActionMove:
            resizeUpdate(action , x , y);
            break;
        case ActionUp:{
            resizeUpdate(action , x , y);
            purple::Log::i("onEventAction RESIZE_CAPTURE_ZONE" , "x = %f , y = %f" , x , y);
            std::vector<float> ps = calClipPoints();
            const int width = static_cast<int>(ps[1] - ps[0]);
            const int height = static_cast<int>(ps[2] - ps[3]);
            mState = (width <= 0 || height <= 0)?Idle:CAPTURE_ZONE_GETTED;
            break;
        }
        default:
            break;
        }//end switch
    }else if(mState == MOVE_CAPTURE_ZONE){ //move capture zone
        switch (action){
        case ActionMove:
            moveCaptureZone(action , x , y);
            break;
        case ActionUp:{
            moveCaptureZone(action , x , y);
            purple::Log::i("onEventAction MOVE_CAPTURE_ZONE" , "x = %f , y = %f" , x , y);
            std::vector<float> ps = calClipPoints();
            const int width = static_cast<int>(ps[1] - ps[0]);
            const int height = static_cast<int>(ps[2] - ps[3]);
            mState = (width <= 0 || height <= 0)?Idle:CAPTURE_ZONE_GETTED;
            break;
        }
        default:
            break;
        }//end switch
    } else if(mState == CAPTURE_ZONE_GETTED){
        if(action == ActionDown && canResetClipZone()){
            //control button click detect
            for(unsigned int i = 0 ; i < mControlButtonRects.size() ;i++){
                purple::Rect &rect = mControlButtonRects[i];
                if(purple::isPointInRect(rect , x , y)){
                    mState = RESIZE_CAPTURE_ZONE;
                    mResizeType = static_cast<ResizeType>(i + 1);
                    purple::Log::i("onTouchEvent" , "on touch control button code: %d" , mResizeType);
                    resizeUpdate(action , x , y);
                    return;
                }
            }//end for i

            // move zone click detect
            std::vector<float> ps = calClipPoints();
            const int width = static_cast<int>(ps[1] - ps[0]);
            const int height = static_cast<int>(ps[2] - ps[3]);
            purple::Rect captureZoneRect(ps[0] , ps[2] , width , height);
            // purple::Log::e("captureZoneRect" , "captureZoneRect detect");
            if(purple::isPointInRect(captureZoneRect , x , y)){
                // purple::Log::e("captureZoneRect" , "captureZoneRect get");
                mState = MOVE_CAPTURE_ZONE;
                moveCaptureZone(action , x , y);
            }
        }//end if    
    }//end if

    // purple::Log::i("onEventAction" , "action = %d ,(%f , %f)" , action , x , y);
}

void Application::resizeUpdate(EventAction action , float x , float y){
    if(mResizeType == None){
        return;
    }

    if(action == EventAction::ActionDown){ //adjusr start pos
        std::vector<float> result = calClipPoints();
        const float left = result[0];
        const float right = result[1];
        const float top = result[2];
        const float bottom = result[3];

        switch(mResizeType){
        case LeftTop:
            mCaptureStartX = right;
            mCaptureStartY = bottom;
            break;
        case RightTop:
            mCaptureStartX = left;
            mCaptureStartY = bottom;
            break;
        case RightBottom:
            mCaptureStartX = left;
            mCaptureStartY = top;
            break;
        case LeftBottom:
            mCaptureStartX = right;
            mCaptureStartY = top;
            break;
        case HmidTop:
            mCaptureStartY = bottom;
            break;
        case HmidBottom:
            mCaptureStartY = top;
            break;
        case LeftVmid:
            mCaptureStartX = right;
            break;
        case RightVmid:
            mCaptureStartX = left;
            break;
        default:
            break;
        }//end switch
    }

    switch(mResizeType){
    case LeftVmid:
    case RightVmid:
        mCaptureEndX = x;
        break;
    case HmidTop:
    case HmidBottom:
        mCaptureEndY = y;
        break;
    case LeftTop:
    case RightTop:
    case RightBottom:
    case LeftBottom:
        mCaptureEndX = x;
        mCaptureEndY = y;
        break;
    default:
        break;
    }//end switch
}

void Application::moveCaptureZone(EventAction action , float x , float y){
    // purple::Log::e("captureZoneRect" , "moveCaptureZone action %d" , action);
    switch(action){
        case ActionDown:
            mLastX = x;
            mLastY = y;
            break;
        case ActionMove:
        case ActionUp:
            adjustMoveCaptureZone(x - mLastX , y - mLastY);
            break;
        default:
            break;
    }//end switch

    mLastX = x;
    mLastY = y;
}

void Application::adjustMoveCaptureZone(float dx , float dy){
    //do limit
    mCaptureStartX += dx;
    mCaptureEndX += dx;
    float left = mCaptureStartX < mCaptureEndX?mCaptureStartX:mCaptureEndX;//left
    if(left < 0.0f){
        mCaptureStartX -= dx;
        mCaptureEndX -= dx;
    }
    float right = mCaptureStartX < mCaptureEndX?mCaptureEndX:mCaptureStartX;//right
    if(right > mScreenWidth){
        mCaptureStartX -= dx;
        mCaptureEndX -= dx;
    }
    
    mCaptureStartY += dy;
    mCaptureEndY += dy;
    float top = mCaptureStartY > mCaptureEndY?mCaptureStartY:mCaptureEndY;//top
    if(top > mScreenHeight){
        mCaptureStartY -= dy;
        mCaptureEndY -= dy;
    }
    float bottom = mCaptureStartY > mCaptureEndY?mCaptureEndY:mCaptureStartY;//bottom
    if(bottom < 0.0f){
        mCaptureStartY -= dy;
        mCaptureEndY -= dy;
    }
}

bool Application::setCurrentEditor(std::shared_ptr<IEditor> editor){
    // if(mState != CAPTURE_ZONE_GETTED || editor == nullptr){
    //     return false;
    // }

    if(mCurrentEditor != nullptr){
        moveEditorToList(mCurrentEditor);
    }

    mCurrentEditor = editor;
    if(mCurrentEditor != nullptr){
        mState = CAPTURE_ZONE_EDIT;
    }
    return true;
}

bool Application::canResetClipZone(){
    return mEditorList.empty();
}

bool Application::onMouseDoubleClick(){
    purple::Log::w("app" , "onMouseDoubleClick ");

    auto confirmMenuItem = mActionMenu->findMenuItemByName(ConfirmMenuItem::Name);
    if(confirmMenuItem != nullptr){
        confirmMenuItem->onItemClick();
        return true;
    }else{
        purple::Log::e("app" , "not found confirmMenuItem");
    }
    return false;
}

void Application::moveEditorToList(std::shared_ptr<IEditor> editor){
    if(editor == nullptr){
        return;
    }

    mEditorList.push_back(editor);
}
