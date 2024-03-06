#pragma once

#include "application.h"

// 编辑接口 
class IEditor{
public:
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
    
    virtual bool dispatchEventAction(EventAction action , float x , float y) override;

    virtual void endPaint();
};

