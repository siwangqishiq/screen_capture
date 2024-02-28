
#include "purple.h"
#include "log.h"
#include "render/render.h"
#include "audio/audio.h"
#include <stdio.h>
#include "widget/timer.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace purple{
    std::shared_ptr<RenderEngine> purple::Engine::renderEngine_ = nullptr;
    std::shared_ptr<Timer> purple::Engine::timer_ = nullptr;

    int purple::Engine::ScreenWidth = 0;
    int purple::Engine::ScreenHeight = 0;
    const char* purple::Engine::TAG = "purple";
    
    void Engine::init(int width , int height){
        Log::w(TAG,"init engine");
        ScreenWidth = width;
        ScreenHeight = height;

        Log::w(TAG,"init screen size: %d  , %d" , ScreenWidth , ScreenHeight);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::e(TAG , "Failed to initialize glad");
            return;
        }
        Log::w(TAG , "glad initialize success.");

        renderEngine_ = std::make_shared<RenderEngine>();
        renderEngine_->init();
        renderEngine_->onScreenResize();

        AudioManager::getInstance()->init();
    }

    void Engine::tick(){
        if(timer_ != nullptr){
            timer_->trick();
        }
    }

    void Engine::dispose(){
        Log::w(TAG,"engine dispose");
        if(renderEngine_ != nullptr){
            renderEngine_->free();
        }
    }

    std::shared_ptr<RenderEngine> Engine::getRenderEngine(){
        return renderEngine_;
    }

    std::shared_ptr<Timer> Engine::getTimer(){
        if(timer_ == nullptr){
            timer_ = std::make_shared<Timer>();
        }

        return timer_;
    }
}