#include "render/cmd/command.h"
#include "render/render.h"
#include "log.h"
#include "render/vram.h"
#include "json.h"
#include "render/common.h"
#include "purple.h"

namespace purple{
    unsigned int RenderCommand::allocatorVRam(int acquireSize , int &allocateSize){
        engine_->vramManager_->fetchVideoMemory(acquireSize , vbo_ ,vao_, vboOffset_ , allocateSize);
        return vbo_;
    }

    void RenderCommand::buildGlCommands(std::vector<float> &buf){
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,buf.size() * sizeof(float) , buf.data());
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }
}



