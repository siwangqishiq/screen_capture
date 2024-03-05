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
        // purple::Log::e("eidtor" , "action move or up  action = %d" , action);
        if(!mVisible){
            return false;
        }

        limitInRect(captureContentRect , x , y);

        if(action == EventAction::ActionMove){
            mEndX = x;
            mEndY = y;
        }else if(action == EventAction::ActionUp){
            mEndX = x;
            mEndY = y;

            endPaint();
        }//end if
        return true;
    }

    return false;
}

//限制坐标在矩形区域内
void RectEditor::limitInRect(purple::Rect &rect , float &x , float &y){
    if(x <= rect.left){
        x = rect.left;
    }else if(x >= rect.getRight()){
        x = rect.getRight();
    }

    if(y >= rect.top){
        y = rect.top;
    }else if(y <= rect.getBottom()){
        y = rect.getBottom();
    }
}

void RectEditor::endPaint(){
    // mApp->mEditorList.push_back(std::shared_ptr<IEditor>(this));
    mApp->setCurrentEditor(std::make_shared<RectEditor>(mApp));

    purple::Log::e("eidtor" , "endPaint mEditorList size = %d" , mApp->mEditorList.size());
}

