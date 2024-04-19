#pragma once

#include "render/cmd/command.h"

namespace purple{
    // triangle render command
    class TrianglesRenderCommand : public RenderCommand{
    public:
        TrianglesRenderCommand(RenderEngine *engine) :RenderCommand(engine){
        }

        virtual void putParams(Shader shader,std::vector<float> &vertex ,Paint &paint ,glm::mat4 &matrix);

        virtual void runCommands() override;
    private:
        Shader shader_;
        Paint paint_;
        glm::mat4 matrix_;
    };
}