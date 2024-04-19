#include "render/cmd/cmd_shape.h"
#include "purple.h"

namespace purple{
     
     void ShapeRenderCommand::putParams(Rect &rect ,Paint &paint, ShapeType type){
        // Logi("ShapeRenderCommand" , "ShapeRenderCommand shape = %d" , type);
        switch(type){
            case ShapeCircle:
            shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_circle"));
            break;
            case ShapeRect:
            shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_rect"));
            break;
            default:
            shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_rect"));
            break;
        }//end switch
        paint_ = paint;
        
        ShaderRenderCommand::putParams(shader_ , rect);
    }

    void ShapeRenderCommand::fillShader(){
        // Logi("shader_" , "shader_ id = %d" , shader_.programId);
        shader_.setUniformVec4("uColor" , paint_.color);
        shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
        shader_.setUniformFloat("uStrokenWidth" , paint_.stokenWidth);
        shader_.setUniformFloat("uWidth" , rect_.width);
        shader_.setUniformFloat("uHeight" , rect_.height);
        shader_.setUniformVec4("uRect" , 
            glm::vec4(rect_.left , rect_.top , rect_.width , rect_.height));
        
        int viewWidth = purple::Engine::ScreenWidth;
        int viewHeight = purple::Engine::ScreenHeight;
        
        shader_.setUniformFloat("uViewWidth" , viewWidth);
        shader_.setUniformFloat("uViewHeight" , viewHeight);
    }
    
}