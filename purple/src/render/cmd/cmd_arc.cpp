#include "render/cmd/cmd_arc.h"

namespace purple{
    void ArcRenderCommand::putParams(Shader shader, Rect &rect 
            , float beginAngle , float endAngle , bool reverse,
            Paint &paint){
        shader_ = shader;
        rect_ = rect;
        angleRange_[0] = beginAngle;
        angleRange_[1] = endAngle;
        reverse_ = reverse;
        paint_ = paint;

        vertexCount_ = 4;
        attrCount_ = 3;

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

        //v2
        vertexBuf[1 * attrCount_ + 0] = rect_.left + rect_.width;
        vertexBuf[1 * attrCount_ + 1] = rect_.top - rect_.height;
        vertexBuf[1 * attrCount_ + 2] = depth;

        //v3
        vertexBuf[2 * attrCount_ + 0] = rect_.left + rect_.width;
        vertexBuf[2 * attrCount_ + 1] = rect_.top;
        vertexBuf[2 * attrCount_ + 2] = depth;

        //v4
        vertexBuf[3 * attrCount_ + 0] = rect_.left;
        vertexBuf[3 * attrCount_ + 1] = rect_.top;
        vertexBuf[3 * attrCount_ + 2] = depth;
        
        buildGlCommands(vertexBuf);
    }

    void ArcRenderCommand::runCommands(){
        if(shader_.programId <= 0){
            return;
        }

        shader_.useShader();
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader_.setUniformVec4("uColor" , paint_.color);
        shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
        shader_.setUniformFloat("uStrokenWidth" , paint_.stokenWidth);
        shader_.setUniformFloat("uReverse" , reverse_? 1.0f: 0.0f);
        // std::cout<< "uAngleRange  " << angleRange_[0] << "   " << angleRange_[1] << std::endl; 
        shader_.setUniformVec2("uAngleRange" , angleRange_[0] , angleRange_[1]);
        shader_.setUniformVec4("uRect" , glm::vec4(rect_.left , rect_.top , rect_.width , rect_.height));

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
            3 * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }
}