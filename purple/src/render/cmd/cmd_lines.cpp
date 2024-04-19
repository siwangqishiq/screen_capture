#include "render/cmd/cmd_lines.h"

namespace purple{
    void LinesRenderCommand::putParams(Shader shader,std::vector<float> &points, Paint &paint){
        shader_ = shader;
        paint_ = paint;

        if(paint.stokenWidth < 1.1f){
            renderByglLines(points);
        }else{
            renderByRects(points);
        }
    }

    void LinesRenderCommand::renderByRects(std::vector<float> &points){
        if(points.size() <= 2){
            return;
        }

        renderByLines = false;

        // vertexCount_ = (points.size()/2 - 1) * 4;
        vertexCount_ = 3 * points.size();
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

        float lineWidth = paint_.stokenWidth;
        float lineHalfWidth = lineWidth / 2.0f;

        glm::vec2 startPoint;
        glm::vec2 endPoint;
        int index = 0;
        const float angle90 = glm::radians(90.0f);

        uint32_t bufIdx = 0;
        while(index < points.size()){
            startPoint = glm::vec2(points[index + 0], points[index + 1]);
            endPoint = glm::vec2(points[index + 2] , points[index + 3]);

            glm::vec2 dir = glm::normalize(endPoint - startPoint);
            
            //起始点 需要考虑线段宽度 再偏移一段距离
            const glm::vec2 adjustVec = lineHalfWidth * dir;
            startPoint += -1.0f * adjustVec;
            endPoint += adjustVec;

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
            
            glm::vec2 p1 = startPoint + topDir * lineHalfWidth;
            glm::vec2 p2 = endPoint + topDir * lineHalfWidth;
            glm::vec2 p3 = endPoint + downDir * lineHalfWidth;
            glm::vec2 p4 = startPoint + downDir * lineHalfWidth;

            vertexBuf[bufIdx++] = p1[0];
            vertexBuf[bufIdx++] = p1[1];
            vertexBuf[bufIdx++] = depth;

            vertexBuf[bufIdx++] = p4[0];
            vertexBuf[bufIdx++] = p4[1];
            vertexBuf[bufIdx++] = depth;

            vertexBuf[bufIdx++] = p2[0];
            vertexBuf[bufIdx++] = p2[1];
            vertexBuf[bufIdx++] = depth;

            vertexBuf[bufIdx++] = p4[0];
            vertexBuf[bufIdx++] = p4[1];
            vertexBuf[bufIdx++] = depth;

            vertexBuf[bufIdx++] = p3[0];
            vertexBuf[bufIdx++] = p3[1];
            vertexBuf[bufIdx++] = depth;

            vertexBuf[bufIdx++] = p2[0];
            vertexBuf[bufIdx++] = p2[1];
            vertexBuf[bufIdx++] = depth;

            index += 4;
        }//end while

        buildGlCommands(vertexBuf);
    }

    void LinesRenderCommand::renderByglLines(std::vector<float> &points){
        renderByLines = true;

        vertexCount_ = points.size() / 2; 
        attrCount_ = 3;

        float depth = engine_->getAndChangeDepthValue();

        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        allocatorVRam(requestSize , allocateSize);

        std::vector<float> buf(vertexCount_ * attrCount_);
        int originIdx = 0;
        for(int i = 0 ; i < vertexCount_ ; i++){
            buf[attrCount_ * i + 0] = points[originIdx++];
            buf[attrCount_ * i + 1] = points[originIdx++];
            buf[attrCount_ * i + 2] = depth;
        }//end for i;
        buildGlCommands(buf);
    }

    void LinesRenderCommand::runCommands(){
        if(shader_.programId <= 0){
            return;
        }

        shader_.useShader();
        shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader_.setUniformVec4("uColor" , paint_.color);

        glLineWidth(paint_.stokenWidth);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
            3 * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glDrawArrays(renderByLines?GL_LINES:GL_TRIANGLES , 0 , vertexCount_);
        // glDrawArrays(renderByLines?GL_LINE_STRIP:GL_TRIANGLES , 0 , vertexCount_);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }
}