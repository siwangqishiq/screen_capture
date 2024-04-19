#pragma once

#include "render/cmd/command.h"
#include "render/cmd/cmd_custom_shader.h"

namespace purple{
    class ShapeRenderCommand : public ShaderRenderCommand {
    public:
        ShapeRenderCommand(RenderEngine *engine) :ShaderRenderCommand(engine){
        }

        void putParams(Rect &rect ,Paint &paint, ShapeType type);

        virtual void fillShader();
    private:
        ShapeType shapeType_ = ShapeRect;
        Paint paint_;
    };
}