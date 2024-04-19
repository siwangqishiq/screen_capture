#pragma once

#include "render/cmd/command.h"

namespace purple{
    class ArcRenderCommand: public RenderCommand{
    public:
        ArcRenderCommand(RenderEngine *engine) 
            :RenderCommand(engine){
        }

        virtual void putParams(Shader shader, Rect &rect 
            , float beginAngle , float endAngle , bool reverse,
            Paint &paint);

        virtual void runCommands() override;
    private:
        Shader shader_;
        Paint paint_;
        Rect rect_;
        glm::vec2 angleRange_; 
        bool reverse_;
    };
}