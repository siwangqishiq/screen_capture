#pragma once

#include "render/cmd/command.h"

namespace purple{
    class LinesSmoothRenderCommand: public RenderCommand{
    public:
        LinesSmoothRenderCommand(RenderEngine *engine) 
            :RenderCommand(engine){
        }

        virtual void putParams(Shader shader,std::vector<float> &points ,Paint &paint);

        virtual void runCommands() override;

        virtual void renderByRects(std::vector<float> &points);
    private:
        Shader shader_;
        Paint paint_;
    };
}