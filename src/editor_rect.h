

#pragma once

#include "editor.h"
#include "application.h"
#include "purple.h"

class RectEditor : public IEditor{
public:
    RectEditor(Application *_app) : mApp(_app){
        mPaint.color = glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.stokenWidth = 6.0f; 
    }

    virtual void renderEditorContent() override;

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;
private:
    Application *mApp;

    float mStartX;
    float mStartY;

    float mEndX;
    float mEndY;

    purple::Paint mPaint;

    bool mVisible = false;
};
