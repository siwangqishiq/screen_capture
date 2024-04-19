#pragma once

#include "render/cmd/command.h"

namespace purple{
    //自定义shader渲染命令
    class ShaderRenderCommand : public RenderCommand {
    public:
        ShaderRenderCommand(RenderEngine *engine) 
            :RenderCommand(engine){
        }

        void putParams(Shader shader, Rect &rect);

        void setPreRenderCallback(std::function<void(void)> callback);
        
        virtual void runCommands();
        
        //set shader params
        virtual void fillShader();

        virtual void buildGlCommands(std::vector<float> &buf);
    protected:
        Shader shader_;
        Rect rect_;
        
        std::function<void(void)> preRenderCallback_ = nullptr;
    };
}