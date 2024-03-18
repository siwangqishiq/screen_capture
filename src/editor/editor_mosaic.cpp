#include "editor/editor_mosaic.h"

void MosaicEditor::renderEditorContent(){
    if(mMosaicTex == nullptr){
        // get texture from cache if not found create it
        mMosaicTex = purple::Engine::getRenderEngine()->buildVirtualTexture("mosaic_texture", 
            mApp->mScreenWidth , mApp->mScreenHeight , 
            [this](int w, int h){
                auto renderEngine = purple::Engine::getRenderEngine();
                purple::Rect dstRect(0.0f , h , w , h);
                renderEngine->renderShader(mShader , dstRect , [](){});
                renderEngine->renderTextureShader(mShader
                    , dstRect
                    , mApp->mScreenImage->getTextureId(), 
                    [this](){
                        mShader.setUniformFloat("uMosaicSize", 8.0f);
                        mShader.setUniformVec2("uTexSize" ,
                            (float)(mApp->mScreenWidth),
                            (float)(mApp->mScreenHeight)
                        );
                        // purple::Log::e("MosaicEditor","setUniform %f , %f" , (float)(mApp->mScreenWidth),(float)(mApp->mScreenHeight));
                    }
                );
            });
    }


    auto batch = purple::Engine::getRenderEngine()->getSpriteBatch();
    batch->begin();
    purple::TextureImage image(mMosaicTex);
    batch->renderImage(image , mRect, mRect);
    batch->end();

    if(isPainting){ //render border
        auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
        shapeBatch->begin();
        shapeBatch->renderRect(mRect , mPaint);
        shapeBatch->end();
    }
}

bool MosaicEditor::dispatchEventAction(EventAction action , float x , float y){
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
            updateMosaicRect();
        }else if(action == EventAction::ActionUp){
            mEndX = x;
            mEndY = y;
            updateMosaicRect();
            endPaint();
        }//end if
        return true;
    }

    return false;
}

void MosaicEditor::updateMosaicRect(){
    float left = mStartX < mEndX?mStartX:mEndX;//left
    float right = mStartX < mEndX?mEndX:mStartX;//right
    float top = mStartY > mEndY?mStartY:mEndY;//top
    float bottom = mStartY > mEndY?mEndY:mStartY;//bottom

    mRect.left = left;
    mRect.top = top;
    mRect.width = right - left;
    mRect.height = top - bottom;
}
    
void MosaicEditor::endPaint(){
    isPainting = false;
    mApp->setCurrentEditor(std::make_shared<MosaicEditor>(mApp));
    purple::Log::w("eidtor" , "endPaint mEditorList size = %d" , mApp->mEditorList.size());
}