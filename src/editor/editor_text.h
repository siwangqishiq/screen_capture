#pragma once

#include "editor/editor.h"
#include "purple.h"

class Application;

class TextEditor : public BaseEditor{
public:
    TextEditor(Application *_app , glm::vec4 color) : BaseEditor(_app){
        mTextColor = color;
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.color = glm::vec4(0.0f , 0.0f ,0.0f , 1.0f);
        mPaint.stokenWidth = 1.0f;

        mDstRect.width = 2.0f * mPadding;
        mDstRect.height = mTextHeight + 2.0f * mPadding;

        std::vector<float> results = mApp->calClipPoints();
        float cx = (results[0] +  results[1]) / 2.0f;
        float cy = (results[2] +  results[3]) / 2.0f;

        mDstRect.left = cx;
        mDstRect.top = cy;
    }

    virtual void setStrokenWidth(float width) override;
    
    virtual void renderEditorContent() override;
    
    virtual void endPaint();

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;
private:
    glm::vec4 mTextColor;
    purple::Rect mDstRect;
    float mPadding = 8.0f;
    float mTextHeight = 32.0f;
};