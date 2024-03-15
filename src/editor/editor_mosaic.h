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
    }

    virtual void renderEditorContent() override;
    
    virtual void endPaint();
private:
    std::shared_ptr<purple::TextureInfo> mMosaicTex = nullptr;
    purple::Shader mShader;
};
