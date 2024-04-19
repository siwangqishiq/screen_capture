#include "render/cmd/cmd_triangles.h"

namespace purple{
     void TrianglesRenderCommand::putParams(Shader shader,std::vector<float> &vertex,
        Paint &paint , glm::mat4 &matrix){
        shader_ = shader;
        matrix_ = matrix;
        
        vertexCount_ = vertex.size();
        const int positionAttr = 3;
        const int colorAttr = 4;
        attrCount_ = positionAttr + colorAttr;
        
        const float depth = engine_->getAndChangeDepthValue();

        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        allocatorVRam(requestSize , allocateSize);
        if(allocateSize < requestSize){
            Log::e("TrianglesRenderCommand" , "TrianglesRenderCommand can not request vram buffer.");
            return;
        }
        std::vector<float> vertexBuf(vertexCount_ * attrCount_);

        for(unsigned int i = 0 ; i < vertexCount_;i += 2){
            unsigned int offset = (i >> 1) * attrCount_;
            //pos
            vertexBuf[offset + 0] = vertex[i + 0];
            vertexBuf[offset + 1] = vertex[i + 1];
            vertexBuf[offset + 2] = depth;
            
            //color
            vertexBuf[offset + 3] = paint.color[0];
            vertexBuf[offset + 4] = paint.color[1];
            vertexBuf[offset + 5] = paint.color[2];
            vertexBuf[offset + 6] = paint.color[3];
        }//end for i

        // std::cout << "vertexBuf = " 
        //     << vertexBuf[7 * 2 + 0] 
        //     <<"," << vertexBuf[7 * 2 + 1]
        //     <<"," << vertexBuf[7 * 2 + 2] << std::endl;
        buildGlCommands(vertexBuf);
    }

    void TrianglesRenderCommand::runCommands(){
        if(shader_.programId <= 0){
            return;
        }
        // Log::e("TrianglesRenderCommand" ,"TrianglesRenderCommand runCommands");

        shader_.useShader();
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader_.setUniformMat4("modelMat" , this->matrix_);

        // std::cout << "error = " << glGetError() << std::endl;

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 4 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
        // std::cout << "vertexCount_ = " << vertexCount_ << std::endl;
    }
}