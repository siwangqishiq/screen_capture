#pragma once

#include "render/cmd/command.h"

namespace purple{
    class LinesRenderCommand: public RenderCommand{
    public:
        LinesRenderCommand(RenderEngine *engine) 
            :RenderCommand(engine){
        }

        virtual void putParams(Shader shader,std::vector<float> &points ,Paint &paint);

        virtual void runCommands() override;

        void renderByglLines(std::vector<float> &points);

        void renderByRects(std::vector<float> &points);
    private:
        Shader shader_;
        Paint paint_;

        bool renderByLines = true;
    };
}