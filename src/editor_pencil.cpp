#include "editor_pencil.h"


void PencilEditor::renderEditorContent(){
    if(!mVisible){
        return;
    }
    
    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    for(auto p : mPoints){
        shapeBatch->renderCircle(p[0] , p[1] , mPencilStokenWidth , mPaint);
    }//end for each
    shapeBatch->end();
}

void PencilEditor::endPaint(){
    mApp->setCurrentEditor(std::make_shared<PencilEditor>(mApp));
    purple::Log::w("eidtor" , "endPaint mEditorList size = %d" , mApp->mEditorList.size());
}

bool PencilEditor::dispatchEventAction(EventAction action , float x , float y) {
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

            mPoints.push_back(glm::vec2(mEndX , mEndY));
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

            mPoints.push_back(glm::vec2(mEndX , mEndY));
        }else if(action == EventAction::ActionUp){
            mEndX = x;
            mEndY = y;

            mPoints.push_back(glm::vec2(mEndX , mEndY));
            endPaint();
        }//end if
        return true;
    }
    return false;
}
