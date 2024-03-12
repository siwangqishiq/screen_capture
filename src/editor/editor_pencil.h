#pragma once

#include "editor/editor.h"
#include "application.h"
#include "purple.h"

class PencilEditor : public BaseEditor{
public:
    PencilEditor(Application *_app , glm::vec4 color , float size) : BaseEditor(_app){
        mPaint.color = color;
        mPaint.fillStyle = purple::FillStyle::Filled;
        mPencilStokenWidth = size / 2.0f;
    }

    void addPoints(float x , float y);

    virtual void renderEditorContent() override;
    
    virtual void endPaint();

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;

    virtual void setStrokenWidth(float width);
private:
    float mMinDistance = 2.0f;
    std::vector<glm::vec2> mPoints;
    float mPencilStokenWidth;
};
