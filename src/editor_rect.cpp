#include "editor_rect.h"
#include "purple.h"

void RectEditor::renderEditorContent(){
    if(!mVisible){
        return;
    }

    float left = mStartX < mEndX?mStartX:mEndX;//left
    float right = mStartX < mEndX?mEndX:mStartX;//right
    float top = mStartY > mEndY?mStartY:mEndY;//top
    float bottom = mStartY > mEndY?mEndY:mStartY;//bottom

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    purple::Rect renderRect(left , top , right - left , top - bottom);
    shapeBatch->renderRect(renderRect , mPaint);
    shapeBatch->end();
}

void RectEditor::endPaint(){
    // mApp->mEditorList.push_back(std::shared_ptr<IEditor>(this));
    mApp->setCurrentEditor(std::make_shared<RectEditor>(mApp , mPaint.color , mPaint.stokenWidth));
    purple::Log::w("eidtor" , "endPaint mEditorList size = %d" , mApp->mEditorList.size());
}

