#pragma once

#include "render/cmd/command.h"

namespace purple{
    class RectRenderCommand: public RenderCommand{
    public:
        RectRenderCommand(RenderEngine *engine) 
            :RenderCommand(engine){
        }

        virtual void putParams(Shader shader, Rect &rect ,glm::mat4 &matrix
            ,Paint &paint);

        virtual void runCommands() override;
    private:
        Shader shader_;
        Rect rect_;
        Paint paint_;

        glm::mat4 modelMat_;
    };
}