#pragma once

#include "application.h"

class Application;

// 编辑接口 
class IEditor{
public:
    virtual void setColor(glm::vec4 color) = 0;

    virtual void setStrokenWidth(float width) = 0;

    virtual void endPaint() = 0;

    virtual void renderEditorContent() = 0; //渲染编辑内容
    
    virtual bool dispatchEventAction(EventAction action , float x , float y) = 0; //路由事件处理
};//end inerface

class BaseEditor : public IEditor{
public:
    BaseEditor(Application *_app) : mApp(_app){}

    Application *mApp;

    float mStartX;
    float mStartY;

    float mEndX;
    float mEndY;

    purple::Paint mPaint;

    bool mVisible = false;

    virtual void limitInRect(purple::Rect &rect , float &x , float &y);
    
    virtual bool dispatchEventAction(EventAction action , float x , float y);

    virtual void endPaint();

    virtual void setColor(glm::vec4 color);

    virtual void setStrokenWidth(float width);
};

