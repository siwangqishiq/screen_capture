#pragma once

#include "editor.h"
#include "application.h"
#include "purple.h"

class PencilEditor : public BaseEditor{
public:
    PencilEditor(Application *_app) : BaseEditor(_app){
        mPaint.color = glm::vec4(0.0f , 0.0f , 1.0f , 1.0f);
        mPaint.fillStyle = purple::FillStyle::Filled;
        mPencilStokenWidth = 6.0f;
    }

    void addPoints(float x , float y);

    virtual void renderEditorContent() override;
    
    virtual void endPaint();

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;
private:
    float mMinDistance = 2.0f;
    std::vector<glm::vec2> mPoints;
    float mPencilStokenWidth;
};
