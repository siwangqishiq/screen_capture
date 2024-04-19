#include "render/cmd/cmd_custom_texture_shader.h"

namespace purple{
    void CustomTextureShaderRenderCommand::putParams(Shader shader, 
        Rect &rect , unsigned int textureId){
        this->shader_ = shader;
        this->rect_ = rect;
        this->textureId_ = textureId;

        vertexCount_ = 4;
        attrCount_ = 3 + 2;

        const float depth = engine_->getAndChangeDepthValue();

        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        allocatorVRam(requestSize , allocateSize);
        if(allocateSize < requestSize){
            Log::i("LinesRenderCommand" , "LinesRenderCommand renderByRects can not request vram buffer.");
            return;
        }
        std::vector<float> vertexBuf(vertexCount_ * attrCount_);

        //v1
        vertexBuf[0 * attrCount_ + 0] = rect_.left;
        vertexBuf[0 * attrCount_ + 1] = rect_.top - rect_.height;
        vertexBuf[0 * attrCount_ + 2] = depth;
        vertexBuf[0 * attrCount_ + 3] = 0.0f;
        vertexBuf[0 * attrCount_ + 4] = 0.0f;

        //v2
        vertexBuf[1 * attrCount_ + 0] = rect_.left + rect_.width;
        vertexBuf[1 * attrCount_ + 1] = rect_.top - rect_.height;
        vertexBuf[1 * attrCount_ + 2] = depth;
        vertexBuf[1 * attrCount_ + 3] = 1.0f;
        vertexBuf[1 * attrCount_ + 4] = 0.0f;

        //v3
        vertexBuf[2 * attrCount_ + 0] = rect_.left + rect_.width;
        vertexBuf[2 * attrCount_ + 1] = rect_.top;
        vertexBuf[2 * attrCount_ + 2] = depth;
        vertexBuf[2 * attrCount_ + 3] = 1.0f;
        vertexBuf[2 * attrCount_ + 4] = 1.0f;

        //v4
        vertexBuf[3 * attrCount_ + 0] = rect_.left;
        vertexBuf[3 * attrCount_ + 1] = rect_.top;
        vertexBuf[3 * attrCount_ + 2] = depth;
        vertexBuf[3 * attrCount_ + 3] = 0.0f;
        vertexBuf[3 * attrCount_ + 4] = 1.0f;

        buildGlCommands(vertexBuf);
    }

    void CustomTextureShaderRenderCommand::runCommands(){
        if(shader_.programId <= 0){
            return;
        }

        //打开混合模式
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

        shader_.useShader();
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);

        glActiveTexture(GL_TEXTURE0);
        shader_.setUniformInt("uTex",0);

        fillShader();
        
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        
        glBindTexture(GL_TEXTURE_2D , textureId_);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}