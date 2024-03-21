
#include "editor/editor_text.h"

void TextEditor::renderEditorContent() {
    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(mDstRect , mPaint);
    shapeBatch->end();
}

void TextEditor::setStrokenWidth(float width){
    //do nothing
}
    
void TextEditor::endPaint() {
    
}

bool TextEditor::dispatchEventAction(EventAction action , float x , float y){
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

        // limitInRect(captureContentRect , x , y);

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