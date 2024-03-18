#pragma once

#include "editor/editor.h"
#include "application.h"
#include "purple.h"

class MosaicEditor : public BaseEditor{
public:
    MosaicEditor(Application *_app) : BaseEditor(_app){
        //init params
        mShader = purple::ShaderManager::getInstance()
            ->loadAssetShader("mosaic_tex_shader","custom/mosaic.vert","custom/mosaic.frag");
        mPaint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.stokenWidth = 1.0f;

        isPainting = true;
    }

    virtual bool dispatchEventAction(EventAction action , float x , float y);

    virtual void renderEditorContent() override;
    
    virtual void updateMosaicRect();

    virtual void endPaint();
private:
    bool isPainting = false;

    std::shared_ptr<purple::TextureInfo> mMosaicTex = nullptr;
    purple::Shader mShader;

    purple::Rect mRect;
};
