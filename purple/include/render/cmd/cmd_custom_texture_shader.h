#pragma once

#include "render/cmd/command.h"
#include "render/cmd/cmd_custom_shader.h"

namespace purple{
    class CustomTextureShaderRenderCommand : public ShaderRenderCommand {
    public:
        CustomTextureShaderRenderCommand(RenderEngine *engine) 
            :ShaderRenderCommand(engine){
        }
        
        void putParams(Shader shader, Rect &rect , unsigned int textureId);

        virtual void runCommands();
    protected:
        unsigned int textureId_;
    };
}