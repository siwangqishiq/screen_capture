#include "render/cmd/cmd_lines_smooth.h"

namespace purple{
    void LinesSmoothRenderCommand::putParams(Shader shader,std::vector<float> &points, Paint &paint){
        shader_ = shader;
        paint_ = paint;
        
        renderByRects(points);
    }

    void LinesSmoothRenderCommand::runCommands() {
        if(shader_.programId <= 0){
            return;
        }

        shader_.useShader();
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader_.setUniformVec4("uColor" , paint_.color);
        shader_.setUniformFloat("uRadius" , paint_.stokenWidth / 2.0f);

        uint32_t attrCountPerVertex = 3 + 2 + 2;

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
            attrCountPerVertex * sizeof(float), 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 
            attrCountPerVertex * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glVertexAttribPointer(2 , 2 , GL_FLOAT , GL_FALSE , 
            attrCountPerVertex * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + (3 + 2) * sizeof(float)));
        
        glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }

    void LinesSmoothRenderCommand::renderByRects(std::vector<float> &points){
        if(points.size() <= 2){
            return;
        }

        vertexCount_ = 3 * points.size();
        attrCount_ = 3 + 2 + 2;

        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        allocatorVRam(requestSize , allocateSize);
        if(allocateSize < requestSize){
            Log::i("SmoothLinesRenderCommand" , "SmoothLinesRenderCommand renderByRects can not request vram buffer.");
            return;
        }

        std::vector<float> vertexBuf(vertexCount_ * attrCount_);

        float lineWidth = paint_.stokenWidth;
        float lineHalfWidth = lineWidth / 2.0f;

        glm::vec2 startPoint;
        glm::vec2 endPoint;
        int index = 0;
        const float angle90 = glm::radians(90.0f);

        uint32_t bufIdx = 0;
        while(index < points.size()){
            glm::vec2 realStartPoint = glm::vec2(points[index + 0], points[index + 1]);
            glm::vec2 realEndPoint = glm::vec2(points[index + 2] , points[index + 3]);

            glm::vec2 dir = glm::normalize(realEndPoint - realStartPoint);
            
            //起始点 需要考虑线段宽度 再偏移一段距离
            const glm::vec2 adjustVec = lineHalfWidth * dir;
            startPoint = realStartPoint -1.0f * adjustVec;
            endPoint = realEndPoint + adjustVec;

            glm::vec2 topDir(
                glm::cos(angle90)* dir[0] + glm::sin(angle90) * dir[1],
                -glm::sin(angle90)* dir[0] + glm::cos(angle90) * dir[1]
            );
            topDir = glm::normalize(topDir);

            glm::vec2 downDir(
                glm::cos(-angle90)* dir[0] + glm::sin(-angle90) * dir[1],
                -glm::sin(-angle90)* dir[0] + glm::cos(-angle90) * dir[1]
            );
            downDir = glm::normalize(downDir);

            // glm::vec2 topDir(0.0f , 1.0f);
            // glm::vec2 downDir(0.0f , -1.0f);

            const float depth = engine_->getAndChangeDepthValue();
            
            glm::vec2 p1 = startPoint + topDir * lineHalfWidth;
            glm::vec2 p2 = endPoint + topDir * lineHalfWidth;
            glm::vec2 p3 = endPoint + downDir * lineHalfWidth;
            glm::vec2 p4 = startPoint + downDir * lineHalfWidth;
            
            vertexBuf[bufIdx++] = p1[0];
            vertexBuf[bufIdx++] = p1[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            vertexBuf[bufIdx++] = p4[0];
            vertexBuf[bufIdx++] = p4[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            vertexBuf[bufIdx++] = p2[0];
            vertexBuf[bufIdx++] = p2[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            vertexBuf[bufIdx++] = p4[0];
            vertexBuf[bufIdx++] = p4[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            vertexBuf[bufIdx++] = p3[0];
            vertexBuf[bufIdx++] = p3[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            vertexBuf[bufIdx++] = p2[0];
            vertexBuf[bufIdx++] = p2[1];
            vertexBuf[bufIdx++] = depth;
            vertexBuf[bufIdx++] = realStartPoint[0];
            vertexBuf[bufIdx++] = realStartPoint[1];
            vertexBuf[bufIdx++] = realEndPoint[0];
            vertexBuf[bufIdx++] = realEndPoint[1];

            index += 4;
        }//end while

        buildGlCommands(vertexBuf);
    }
}