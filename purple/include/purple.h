
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
    private:
        static std::shared_ptr<RenderEngine> renderEngine_;
        static std::shared_ptr<Timer> timer_;
    };
}

