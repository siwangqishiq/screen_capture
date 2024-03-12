#include "editor/editor_pencil.h"


void PencilEditor::renderEditorContent(){
    if(!mVisible){
        return;
    }
    
    // auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    // shapeBatch->begin();
    // for(auto p : mPoints){
    //     shapeBatch->renderCircle(p[0] , p[1] , mPencilStokenWidth , mPaint);
    // }//end for each
    // shapeBatch->end();

    purple::Engine::getRenderEngine()->renderLines(mPaintPoints , mPaint);
}

void PencilEditor::endPaint(){
    mApp->setCurrentEditor(std::make_shared<PencilEditor>(mApp , mPaint.color , mPaint.stokenWidth));
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

            mPaintPoints.push_back(mStartX);
            mPaintPoints.push_back(mStartY);
            mPaintPoints.push_back(mEndX);
            mPaintPoints.push_back(mEndY);

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
            addPoints(x , y);

            mEndX = x;
            mEndY = y;
        }else if(action == EventAction::ActionUp){
            addPoints(x , y);

            mEndX = x;
            mEndY = y;

            mPoints.push_back(glm::vec2(mEndX , mEndY));
            endPaint();
        }//end if
        return true;
    }
    return false;
}

void PencilEditor::addPoints(float x , float y){
    // glm::vec2 startPos(mEndX , mEndY);
    // glm::vec2 curPos = startPos;
    // glm::vec2 endPos(x, y);
    // glm::vec2 dirNormal = glm::normalize(endPos - startPos);
    
    // while(glm::distance(curPos , endPos) > mMinDistance){
    //     curPos = startPos + dirNormal * mMinDistance;
    //     mPoints.push_back(curPos);
    //     startPos = curPos;
    // }//end while

    mPaintPoints.push_back(mEndX);
    mPaintPoints.push_back(mEndY);
    mPaintPoints.push_back(x);
    mPaintPoints.push_back(y);
}

void PencilEditor::setStrokenWidth(float width){
    // mPencilStokenWidth = width / 2.0f;
    mPaint.stokenWidth = width;
}
