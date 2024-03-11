

#pragma once

#include "editor.h"
#include "application.h"
#include "purple.h"

class RectEditor : public BaseEditor{
public:
    RectEditor(Application *_app , glm::vec4 color , float size) : BaseEditor(_app){
        mPaint.color = color;
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.stokenWidth = size;
    }
    
    virtual void renderEditorContent() override;
    
    virtual void endPaint();
};
