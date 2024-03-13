
#pragma once

#include "log.h"
#include <memory>
#include "render/common.h"
#include "render/render.h"
#include "render/render_batch.h"
#include "widget/timer.h"
#include "render/texture.h"
#include "render/sprite.h"
#include "render/common.h"
#include "json.h"


namespace purple{
    
    class RenderEngine;
    class Engine{
    public:
        static const char *TAG;
        static int ScreenWidth;
        static int ScreenHeight;
        
        static void init(int width , int height);

        static void dispose();

        static void tick();

        static std::shared_ptr<RenderEngine> getRenderEngine();

        static std::shared_ptr<Timer> getTimer();
        
        //导出图片
        static int exportImageFile(std::string path ,int left , int top , 
                int width , int height);
    private:
        static std::shared_ptr<RenderEngine> renderEngine_;
        static std::shared_ptr<Timer> timer_;
    };
}

