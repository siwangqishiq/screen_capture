#include "render/cmd/cmd_rect.h"

namespace purple{
    void RectRenderCommand::putParams(Shader shader,
                Rect &rect, 
                glm::mat4 &matrix ,Paint &paint){
        shader_ = shader;
        rect_ = rect;
        paint_ = paint;
        modelMat_ = matrix;

        vertexCount_ = 4; //4个顶点
        attrCount_ = 3;//每个顶点3个属性长度

        float depth = engine_->getAndChangeDepthValue();

        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        allocatorVRam(requestSize , allocateSize);
        std::vector<float> buf(vertexCount_ * attrCount_);
        
        //v1
        buf[0 * 3 + 0] = rect.left;
        buf[0 * 3 + 1] = rect.top - rect.height;
        buf[0 * 3 + 2] = depth;

        //v2
        buf[1 * 3 + 0] = rect.left + rect.width;
        buf[1 * 3 + 1] = rect.top - rect.height;
        buf[1 * 3 + 2] = depth;

        //v3
        buf[2 * 3 + 0] = rect.left + rect.width;
        buf[2 * 3 + 1] = rect.top;
        buf[2 * 3 + 2] = depth;

        //v4
        buf[3 * 3 + 0] = rect.left;
        buf[3 * 3 + 1] = rect.top;
        buf[3 * 3 + 2] = depth;

        buildGlCommands(buf);
    }

    void RectRenderCommand::runCommands(){
        if(shader_.programId <= 0){
            return;
        }
        
        shader_.useShader();
        shader_.setUniformMat4("modelMat" , modelMat_);
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader_.setUniformVec4("uColor" , paint_.color);
        shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
        // fillShader();

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }
    
}