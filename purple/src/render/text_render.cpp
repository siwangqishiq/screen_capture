
#include "render/text_render.h"
#include "render/render.h"
#include "resource/asset_manager.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "render/stb_truetype.h"

namespace purple{
    void TextRender::renderText(std::wstring text , 
            float left , 
            float bottom , 
            TextPaint &paint){
        //todo render text
        // Log::i(TAG , "start render text");

        buildTextRenderVertexData(text , left , bottom , paint);
        submitRenderCommand();
    }

    void TextRender::buildTextRenderVertexData(std::wstring &text , 
                float left , 
                float bottom , 
                TextPaint &textPaint){
        //vertex data pos (x , y ,depth) , textureCoords [uvw] 
        float depth = renderEngine_->getAndChangeDepthValue();
        const float fontHeight = textPaint.getTextFontHeight();
        for(wchar_t ch : text){
            
        }//end for each
    }

    void TextRender::submitRenderCommand(){
        
    }

    void TextRender::renderText(const wchar_t *text, 
            float left, 
            float bottom, 
            TextPaint &paint){
        renderText(std::wstring(text) , left , bottom , paint);
    }

    int TextRender::loadFontRes(std::string fontName, std::string fontFileAssetPath){
        int fontFileSize = 0;
        fontData_ = AssetManager::getInstance()->readFileAsBinRaw(fontFileAssetPath , fontFileSize);
        if(fontFileSize <= 0){
            Log::e(TAG , "Not found font file %s" , fontFileAssetPath.c_str());
            return -1;
        }
        Log::i(TAG , "%s file size : %d" , fontFileAssetPath.c_str() , fontFileSize);
        
        int ret = 0;
        ret = initFont();

        return ret; 
    }

    int TextRender::initFont(){
        int errCode = stbtt_InitFont(&fontInfo_ , fontData_ , 0);
        fontScale_ = stbtt_ScaleForPixelHeight(&fontInfo_ , FONT_DEFAULT_SIZE);
        Log::i(TAG , "fontScale =  %f" , fontScale_);
        return errCode;
    }
}

