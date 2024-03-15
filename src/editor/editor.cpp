#include "editor/editor.h"
#include "application.h"

//限制坐标在矩形区域内
void BaseEditor::limitInRect(purple::Rect &rect , float &x , float &y){
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

bool BaseEditor::isVisible(){
    return mVisible;
}

bool BaseEditor::dispatchEventAction(EventAction action , float x , float y){
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

void BaseEditor::endPaint(){
    //do nothing wait sub class
}

void BaseEditor::setColor(glm::vec4 color){
    mPaint.color = color;
}

void BaseEditor::setStrokenWidth(float width){
    mPaint.stokenWidth = width;
}