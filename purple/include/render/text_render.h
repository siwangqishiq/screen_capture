#pragma once

#include <string>
#include "render/common.h"
#include "log.h"
#include "render/stb_truetype.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace purple{
    class RenderEngine;
    class TextureInfo;
    class SdfTextRenderCommand;

    struct TextRenderOutInfo{
        Rect outRect;
        int renderTextSize;
    };
    
    class TextRender{
    public:
        const std::string TAG = "TextRender";

        const int CHARS_MAX_COUNT_PER_COLUM = 16;

        const int texWidth_ = CHARS_MAX_COUNT_PER_COLUM * FONT_DEFAULT_SIZE;
        const int texHeight_ = CHARS_MAX_COUNT_PER_COLUM * FONT_DEFAULT_SIZE;
        const int texDepth_ = 32;

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
        
        int loadFontRes(std::string fontName , 
                        std::string fontFileAssetPath , 
                        bool isAssetRes = false);

        void renderText(const wchar_t *text, 
            float left , 
            float baseline, 
            TextPaint &textPaint);
        
        void renderText(std::wstring text , 
            float left , float baseline, 
            TextPaint &textPaint);
        
        void renderTextWithRect(
            std::wstring text , 
            Rect &showRect ,
            TextPaint &textPaint ,
            TextRenderOutInfo *outInfo);

        std::shared_ptr<CharInfo> findCharInfo(wchar_t &ch , int index);

        stbtt_fontinfo* getFontInfo(){
            return &fontInfo_;
        }

        float getFonScale(){
            return fontScale_;
        }

        std::shared_ptr<TextureInfo> getFontTextureInfo(){
            return fontTextureInfo_;
        }

        void layoutText(std::wstring &content, 
                            SdfTextRenderCommand &renderCmd,
                            TextRenderOutInfo &outRect,
                            std::vector<float> &buf);
    private:

        RenderEngine *renderEngine_;

        const unsigned char *fontData_ = nullptr;
        stbtt_fontinfo fontInfo_;

        float fontScale_ = 1.0f;

        int initFont();

        //create 3D texture
        int createFontTextureRes();

        std::vector<float> addBitmapToTextures(unsigned char *bitmap , 
            int width , int height);

        void updateNextOffset();

        void putCoords(CharInfo &info , std::vector<float> &coords) const;

        std::unordered_map<wchar_t , std::shared_ptr<CharInfo>> charInfoMap_;
        
        std::shared_ptr<TextureInfo> fontTextureInfo_;

        std::string fontName_;

        int offsetX_ = 0;
        int offsetY_ = 0;
        int offsetZ_ = 0;

        int fontWidth_ = FONT_DEFAULT_SIZE;
        int fontHeight_ = FONT_DEFAULT_SIZE;
    };
} // namespace  



