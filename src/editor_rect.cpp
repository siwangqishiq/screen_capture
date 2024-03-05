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

bool RectEditor::dispatchEventAction(EventAction action , float x , float y){
    std::vector<float> results = mApp->calClipPoints();
    float left = results[0];
    float right = results[1];
    float top = results[2];
    float bottom = results[3];

    purple::Rect captureContentRect(left , top , right - left , top - bottom);

    if(action == EventAction::ActionDown){
        if(purple::isPointInRect(captureContentRect , x , y)){
            mStartX = x;
            mStartY = y;
            mEndX = x;
            mEndY = y;
            mVisible = true;
            return true;
        }
    }else{
        if(!mVisible){
            return false;
        }

        if(x <= left){
            x = left;
        }else if(x >= right){
            x = right;
        }

        if(y >= top){
            y = top;
        }else if(y <= bottom){
            y = bottom;
        }

        if(action == EventAction::ActionMove){
            mEndX = x;
            mEndY = y;
        }else if(action == EventAction::ActionUp){
            mEndX = x;
            mEndY = y;
        }//end if

        return true;
    }

    return false;
}

