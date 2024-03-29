#include "editor/editor_circle.h"

void CircleEditor::renderEditorContent(){
    if(!mVisible){
        return;
    }
    
    float left = mStartX < mEndX?mStartX:mEndX;//left
    float right = mStartX < mEndX?mEndX:mStartX;//right
    float top = mStartY > mEndY?mStartY:mEndY;//top
    float bottom = mStartY > mEndY?mEndY:mStartY;//bottom

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    // mPaint.fillStyle = purple::FillStyle::Filled;
    shapeBatch->begin();
    purple::Rect renderRect(left , top , right - left , top - bottom);
    shapeBatch->renderOval(renderRect , mPaint);
    shapeBatch->end();
}

void CircleEditor::endPaint(){
    mApp->setCurrentEditor(std::make_shared<CircleEditor>(mApp , mPaint.color , mPaint.stokenWidth));
    purple::Log::w("eidtor" , "endPaint mEditorList size = %d" , mApp->mEditorList.size());
}

int CircleEditor::editorType(){
    return EditorType::DrawOval;
}