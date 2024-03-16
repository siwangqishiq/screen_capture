#include "editor/editor_mosaic.h"

void MosaicEditor::renderEditorContent(){
    if(mMosaicTex == nullptr){
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
                        (float)(mApp->mScreenWidth),(float)(mApp->mScreenHeight));
                        // purple::Log::e("MosaicEditor","setUniform %f , %f" , (float)(mApp->mScreenWidth),(float)(mApp->mScreenHeight));
                    }
                );
            });
    }
    
    auto batch = purple::Engine::getRenderEngine()->getSpriteBatch();
    batch->begin();
    purple::TextureImage image(mMosaicTex);

    std::vector<float> ps = mApp->calClipPoints();
    purple::Rect imageRect(ps[0] , ps[2] , ps[1] - ps[0] , ps[2] - ps[3]);
    batch->renderImage(image , imageRect, imageRect);
    batch->end();
}
    
void MosaicEditor::endPaint(){

}