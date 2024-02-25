
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

    window = glfwCreateWindow(mScreenWidth, mScreenHeight, "screen capture", nullptr, nullptr);
    
    if (window == nullptr) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowPos(window , 0 , 0);
    glfwSetWindowAttrib(window , GLFW_DECORATED , GLFW_FALSE);

    glfwSetKeyCallback(window , [](GLFWwindow* windows_,int key,int scancode,int action,int mods){
        if(glfwGetKey(windows_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(windows_, true);
        }
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

    image = purple::BuildImageByAsset(std::string("t2.jpg"));

    mScreenImage = purple::BuildImageByPixlData(mScreenImagePixel , mScreenWidth , mScreenHeight , GL_RGB);
}

void Application::tick(){
    purple::Engine::tick();


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

    purple::Rect maskRect = imgDstRect;
    purple::Paint maskPaint;
    maskPaint.color = glm::vec4(0.0f ,0.0f ,0.0f , 0.4f);
    auto shapeBatch  = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(maskRect , maskPaint);
    shapeBatch->end();
}