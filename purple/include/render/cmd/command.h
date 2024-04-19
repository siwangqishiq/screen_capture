#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "render/common.h"
#include "render/render.h"
#include <vector>
#include "render/shader.h"
#include <functional>

#include "log.h"

namespace purple{
    class RenderEngine;
    struct CharInfo;

    

    class RenderCommand{
    public:
        RenderEngine *engine_;
        RenderCommand(RenderEngine *engine) : engine_(engine){}

        virtual void runCommands(){}

        bool used = false;

        unsigned int allocatorVRam(int acquireSize , int &allocateSize);

        int vertexCount_;
        int attrCount_;

        virtual void buildGlCommands(std::vector<float> &buf);
    protected:
        unsigned int vbo_;
        unsigned int vao_;

        int vboOffset_;
    };
}




