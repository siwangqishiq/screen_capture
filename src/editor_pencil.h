#pragma once

#include "editor.h"
#include "application.h"
#include "purple.h"

class PencilEditor : public BaseEditor{
public:
    PencilEditor(Application *_app) : BaseEditor(_app){
        mPaint.color = glm::vec4(0.0f , 0.0f , 1.0f , 1.0f);
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.stokenWidth = 6.0f; 
    }

    virtual void renderEditorContent() override;
    
    virtual void endPaint();
};
