#pragma once

#include "render/cmd/command.h"

namespace purple{
    // SDF文本渲染命令
    class SdfTextRenderCommand : public RenderCommand{
    public:
        SdfTextRenderCommand(RenderEngine *engine) : RenderCommand(engine){}
    };
}

