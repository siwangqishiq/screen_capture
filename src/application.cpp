
#include "application.h"
#include "bridge.h"
#include "purple.h"

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
        if(event == GLFW_PRESS){
            app_->mMouseActionDown = true;
            app_->onEventAction(ActionDown , app_->mEventX 
                , app_->mScreenHeight - app_->mEventY);
        }else if(event == GLFW_RELEASE){
            app_->mMouseActionDown = false;
            app_->onEventAction(ActionUp , app_->mEventX 
                , app_->mScreenHeight - app_->mEventY);
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
    
    purple::Engine::init(mScreenWidth , mScreenHeight);
    init();
    while(!glfwWindowShouldClose(window)) {
        tick();
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(1);//锁定固定帧率
    }//end while

    purple::Engine::dispose();
    glfwTerminate();
}

void Application::init(){
    purple::Log::i("purple_engine" , "init");

    // image = purple::BuildImageByAsset(std::string("t2.jpg"));
    mMaskZonePaint = purple::Paint();
    mMaskZonePaint.color = glm::vec4(0.0f , 0.0f ,0.0f , 0.45f);

    mScreenImage = purple::BuildImageByPixlData(mScreenImagePixel , mScreenWidth , mScreenHeight , GL_RGB);
}

void Application::tick(){
    purple::Engine::tick();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw bottom image (screen image)
    renderScreenCaptureImage();
    renderMaskZone();

    // purple::Rect maskRect = imgDstRect;
    // purple::Paint maskPaint;
    // maskPaint.color = glm::vec4(0.0f ,0.0f ,0.0f , 0.4f);
    // auto shapeBatch  = purple::Engine::getRenderEngine()->getShapeBatch();
    // shapeBatch->begin();
    // shapeBatch->renderRect(maskRect , maskPaint);
    // shapeBatch->end();
}

void Application::renderScreenCaptureImage(){
    purple::Rect imgDstRect;
    imgDstRect.left = 0.0f;
    imgDstRect.top = purple::ScreenHeight;
    imgDstRect.width = purple::ScreenWidth;
    imgDstRect.height = purple::ScreenHeight;
    auto spriteBatch  = purple::Engine::getRenderEngine()->getSpriteBatch();

    spriteBatch->begin();
    auto src = mScreenImage->getRect();
    spriteBatch->renderImage(*mScreenImage , src , imgDstRect);
    spriteBatch->end();
}

void Application::renderMaskZone(){
    const float left = mCaptureStartX < mCaptureEndX?mCaptureStartX:mCaptureEndX;
    const float right = mCaptureStartX < mCaptureEndX?mCaptureEndX:mCaptureStartX;
    const float top = mCaptureStartY > mCaptureEndY?mCaptureStartY:mCaptureEndY;
    const float bottom = mCaptureStartY > mCaptureEndY?mCaptureEndY:mCaptureStartY;
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

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(topRect , mMaskZonePaint);
    shapeBatch->renderRect(bottomRect , mMaskZonePaint);
    shapeBatch->renderRect(leftRect , mMaskZonePaint);
    shapeBatch->renderRect(rightRect , mMaskZonePaint);
    shapeBatch->end();
}

void Application::onResize(int w , int h){
    purple::Log::i("resize" , "width = %d , height = %d" , w , h);
}

void Application::onEventAction(EventAction action , float x , float y){
    if(mState == Idle){ // idle空闲状态
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
            mState = Idle;
            break;
        default:
            break;
        }//end switch
    }
    // purple::Log::i("onEventAction" , "action = %d ,(%f , %f)" , action , x , y);
}