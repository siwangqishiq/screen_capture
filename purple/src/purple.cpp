
#include "purple.h"
#include "log.h"
#include "render/render.h"
#include "audio/audio.h"
#include <stdio.h>
#include "widget/timer.h"
#include <algorithm>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace purple{
    std::shared_ptr<RenderEngine> purple::Engine::renderEngine_ = nullptr;
    std::shared_ptr<Timer> purple::Engine::timer_ = nullptr;

    int purple::Engine::ScreenWidth = 0;
    int purple::Engine::ScreenHeight = 0;
    const char* purple::Engine::TAG = "purple";
    
    void Engine::init(int width , int height){
        Log::w(TAG,"init engine");
        long startTime = currentTimeMillis();
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

        long endTime = currentTimeMillis();
        Log::w(TAG , "purple engine start time : %ld ms" , endTime - startTime);
    }
    
    void Engine::resize(int w , int h){
        if(ScreenWidth == w && ScreenHeight == h){
            return;
        }

        ScreenWidth = w;
        ScreenHeight = h;
        Log::w(TAG , "scren resize %d x %d" , w , h);
        if(renderEngine_ == nullptr){
            renderEngine_ = std::make_shared<RenderEngine>();
        }
        renderEngine_->onScreenResize();
    }

    void Engine::tick(){
        if(timer_ != nullptr){
            timer_->trick();
        }

        if(renderEngine_->vramManager_ != nullptr){ //显存缓存清空
            renderEngine_->vramManager_->onPostRender();
            renderEngine_->getShapeBatch()->getVRamManager()->onPostRender();
            renderEngine_->getSpriteBatch()->getVRamManager()->onPostRender();
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

    //将引擎绘制内容 导出为图片
    int Engine::exportImageFile(std::string path ,int left , int bottom, int width , int height){
        Log::i("purple_engine" , "export image : %d, %d , %d , %d"
            , left , bottom , width , height);
        if(left < 0 || bottom < 0 || width <= 0 || height <= 0){
            Log::e("purple_engine" , "export image error invalied param : %d, %d , %d , %d"
                , left , bottom , width , height);
            return -1;
        }

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadBuffer(GL_BACK_LEFT);
        const int channel = 3;
        const int size = channel * width * height;
        std::vector<uint8_t> pixels(size);
        glReadPixels(left , bottom , width , height,
                    GL_RGB , GL_UNSIGNED_BYTE , pixels.data());
        for(int line = 0; line != height/2; ++line) {
            std::swap_ranges(pixels.begin() + 3 * width * line,
                             pixels.begin() + 3 * width * (line+1),
                             pixels.begin() + 3 * width * (height-line-1));
        }//end for line
        stbi_write_png(path.c_str() , width, height, channel , pixels.data() , channel * width);
        return 0;    
    }
}