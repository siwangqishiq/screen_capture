#pragma once

#include "application.h"

class Application;

enum EditorType{
    NoneType = -1,
    DrawRect = 1,
    DrawOval = 2,
    DrawFree = 3,
    DrawMosaic = 4,
    DrawText = 5
};

// 编辑接口 
class IEditor{
public:
    virtual void setColor(glm::vec4 color) = 0;

    virtual void setStrokenWidth(float width) = 0;

    virtual void endPaint() = 0;

    virtual bool isVisible() = 0;

    virtual void renderEditorContent() = 0; //渲染编辑内容
    
    virtual bool dispatchEventAction(EventAction action , float x , float y) = 0; //路由事件处理

    virtual void onInputContentChange(std::wstring content) = 0;

    virtual int editorType() = 0;
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

    virtual bool isVisible();

    virtual void limitInRect(purple::Rect &rect , float &x , float &y);
    
    virtual bool dispatchEventAction(EventAction action , float x , float y);

    virtual void endPaint();

    virtual void setColor(glm::vec4 color);
    
    virtual void setStrokenWidth(float width);

    virtual void onInputContentChange(std::wstring content);

    virtual int editorType();
};

