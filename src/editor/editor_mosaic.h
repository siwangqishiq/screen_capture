#pragma once

#include "editor/editor.h"
#include "application.h"
#include "purple.h"

class MosaicEditor : public BaseEditor{
public:
    MosaicEditor(Application *_app) : BaseEditor(_app){
        // mPaint.color = color;
        // mPaint.fillStyle = purple::FillStyle::Stroken;
        // mPaint.stokenWidth = size; 
    }

    virtual void renderEditorContent() override;
    
    virtual void endPaint();
};
