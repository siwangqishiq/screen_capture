#pragma once

#include "editor/editor.h"
#include "application.h"
#include "purple.h"

class CircleEditor : public BaseEditor{
public:
    CircleEditor(Application *_app , glm::vec4 color, float size) : BaseEditor(_app){
        mPaint.color = color;
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.stokenWidth = size; 
    }

    virtual void renderEditorContent() override;
    
    virtual void endPaint();
};
