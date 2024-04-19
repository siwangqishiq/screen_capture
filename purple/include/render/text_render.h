#pragma once

#include <string>
#include "render/common.h"
#include "log.h"
#include "render/stb_truetype.h"
#include <memory>

namespace purple{
    class RenderEngine;

    class TextRender{
    public:
        const std::string TAG = "TextRender";

        TextRender(RenderEngine *engine):renderEngine_(engine){
            Log::i(TAG , "TextRender construct");
        };

        ~TextRender(){
            if(fontData_ != nullptr){
                delete[] fontData_;
                fontData_ = nullptr;
            }
            Log::i(TAG , "TextRender deconstructed");
        };
        
        int loadFontRes(std::string fontName , std::string fontFileAssetPath);

        void renderText(const wchar_t *text, 
            float left , float baseLine, 
            TextPaint &textPaint);
        
        void renderText(std::wstring text , 
            float left , float baseLine, 
            TextPaint &textPaint);
    private:

        RenderEngine *renderEngine_;

        const unsigned char *fontData_ = nullptr;
        stbtt_fontinfo fontInfo_;

        float fontScale_ = 1.0f;

        int initFont();

        void buildTextRenderVertexData(std::wstring &text , 
                float left , 
                float top , 
                TextPaint &paint);
        
        void submitRenderCommand();
    };
} // namespace  



