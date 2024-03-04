#pragma once

#include "application.h"

// 编辑接口 
class IEditor{
public:
    virtual void renderEditorContent() = 0; //渲染编辑内容

    virtual bool dispatchEventAction(EventAction action , float x , float y) = 0; //路由事件处理
};//end inerface

