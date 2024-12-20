
#include "render/text_render.h"
#include "render/render.h"
#include "resource/asset_manager.h"

#include "render/cmd/cmd_text.h"
#include "render/cmd/cmd_text_sdf.h"
#include "render/texture.h"
#include "glheader.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "render/stb_truetype.h"

namespace purple{
    void TextRender::renderTextWithRect(
            std::wstring text , 
            Rect &showRect ,
            TextPaint &textPaint , 
            TextRenderOutInfo *outInfo
        )
    {
        SdfTextRenderCommand cmd(renderEngine_ , this);
        cmd.limitRect_ = showRect;
        auto ret =  cmd.putTextParamsByRectLimit(text , showRect ,outInfo, textPaint);
        if(ret >= 0){
            cmd.runCommands();
        }
    }

    void TextRender::preCalTextRect(std::wstring text, 
            TextPaint &textPaint,int maxWidth,Rect &outInfo){
        if(text.empty()){
            outInfo.width = 0;
            outInfo.height = 0;
            return;
        }

        const int size = text.length();
        const int oneLineHeight = (FONT_DEFAULT_SIZE + textPaint.gapSize) * textPaint.textSizeScale;

        int index = 0;    
        int x = 0;
        int y = oneLineHeight;
        int lineWidth = 0;
        
        while(index < size){
            // Log::i("test","size = %d index = %d", size , index);
            wchar_t ch = text[index++];
            if(ch == L'\n'){
                y += oneLineHeight;
                x = 0;
                continue;
            }

            auto charInfoPtr = findCharInfo(ch , index);
            if(charInfoPtr == nullptr){
                continue;
            }

            const float charRealWidth = (charInfoPtr->width + textPaint.gapSize) * textPaint.textSizeScale;
            if(x + charRealWidth <= maxWidth){
                x += charRealWidth;
                if(outInfo.width < x){
                    outInfo.width = x;
                }
            }else{// change a new line
                x = 0;
                y += oneLineHeight;
            }//end if
        }//end while

        if(outInfo.width < x){
            outInfo.width = x;
        }
        outInfo.height = y;
    }

    void TextRender::renderText(std::wstring text , 
            float left , 
            float baseline , 
            TextPaint &paint){
        SdfTextRenderCommand cmd(renderEngine_ , this);
        cmd.putParams(text , left , baseline , paint);
        cmd.runCommands();
    }

    int TextRender::createFontTextureRes(){
        fontTextureInfo_ = 
        TextureManager::getInstance()->createEmptyTexture2dArray(fontName_,
            texWidth_ , texHeight_ , texDepth_ , GL_RED);
        return fontTextureInfo_->textureId;
    }

    std::shared_ptr<CharInfo> TextRender::findCharInfo(wchar_t &ch , int index){
        if(charInfoMap_.find(ch) != charInfoMap_.end()){ //hit cache 
            return charInfoMap_[ch];
        }

        //create
        std::shared_ptr<CharInfo> result = std::make_shared<CharInfo>();

        int width = 0;
        int height = 0;
        int offX = 0;
        int offY = 0;

        if(ch == L' '){
            width = SPACE_WIDTH;
        }

        unsigned char* sdfBitmap = stbtt_GetCodepointSDF(
            &fontInfo_, fontScale_, 
            ch , 0, 128, 128/5.0f,
            &width, &height , &offX, &offY);
        if(sdfBitmap == nullptr || width <= 0 || height <= 0){
            Log::e(TAG ,"sdf data is null index = %d! width = %d , height = %d" 
                , index , width, height);
        }
        
        std::vector<float> coords = addBitmapToTextures(sdfBitmap , width, height);
        if(sdfBitmap != nullptr){
            stbtt_FreeSDF(sdfBitmap , nullptr);
            sdfBitmap = nullptr;
        }

        // Log::e(TAG , "char width = %d , height = %d , offx = %d , offy = %d" 
        //     , width , height , offX , offY);

        result->value = ch;
        result->width = width;
        result->height = height;
        result->bearingX = offX;
        result->bearingY = -offY;
        result->textureId = fontTextureInfo_->textureId;
        putCoords(*result , coords);

        //add cache
        charInfoMap_[ch] = result;
        return result;
    }

    void TextRender::updateNextOffset(){
        if(offsetX_ + fontWidth_ >= texWidth_){
            if(offsetY_ + fontHeight_ >= texHeight_){
                if(offsetZ_ + 1 > texDepth_){
                    offsetZ_ = 1;//第0页 考虑必然是常用字符 不做替换 从第二页开始
                }else{
                    offsetZ_ += 1;
                }
                offsetY_ = 0;
            }else{
                offsetY_ += fontHeight_;
            }
            offsetX_ = 0;
        }else{
            offsetX_ += fontWidth_;
        }

        // Log::i(TAG , "updateNextOffset offsetX : %d , offsetY:%d , offsetZ : %d",
        //     offsetX_ , offsetY_ , offsetZ_);
    }

    //设置纹理坐标信息
    void TextRender::putCoords(CharInfo &info , std::vector<float> &coords) const{
        if(coords.empty()){
            return;
        }

        for(int i = 0 ; i < 5 ; i++){
            info.textureCoords[i] = coords[i];
            // Log::i(TAG , "textureCoords[%d] = %f" , i , info.textureCoords[i]);
        }//end for i
    }

    std::vector<float> TextRender::addBitmapToTextures(unsigned char *bitmap, 
            int width , 
            int height){
        updateNextOffset();//update offset data

        // Log::i(TAG ,"copy sdf data to texture 2d array cur off_x : %d , off_y : %d , off_z : %d w :%d , h : %d",
        //     this->offsetX_ , this->offsetY_ , offsetZ_ , width , height);
        // fontTextureInfo_

        if(bitmap != nullptr && width != 0 && height != 0){
            TextureManager::getInstance()->updateTexture2dArrayData(
                fontTextureInfo_ , 
                offsetX_ , 
                offsetY_ ,
                offsetZ_ , 
                width , height , 1 , bitmap);
        }
    
        
        std::vector<float> texCoords(5);
        texCoords[0] = static_cast<float>(offsetX_) / static_cast<float>(texWidth_);
        texCoords[1] = static_cast<float>(offsetY_) / static_cast<float>(texHeight_);
        texCoords[2] = static_cast<float>(offsetX_ + width) / static_cast<float>(texWidth_);
        texCoords[3] = static_cast<float>(offsetY_ + height) / static_cast<float>(texHeight_);
        texCoords[4] = static_cast<float>(offsetZ_);

        return texCoords;
    }

    void TextRender::renderText(const wchar_t *text, 
            float left, 
            float baseline, 
            TextPaint &paint){
        renderText(std::wstring(text) , left , baseline , paint);
    }

    int TextRender::loadFontRes(std::string fontName, std::string fontFileAssetPath , bool isAssetRes){
        int fontFileSize = 0;
        if(isAssetRes){
            fontData_ = AssetManager::getInstance()->readAssetFileAsBinRaw(fontFileAssetPath , fontFileSize);
        }else{
            fontData_ = AssetManager::getInstance()->readFileAsBinRaw(fontFileAssetPath , fontFileSize);
        }
        
        if(fontFileSize <= 0){
            Log::e(TAG , "Not found font file %s" , fontFileAssetPath.c_str());
            return -1;
        }
        Log::i(TAG , "%s file size : %d" , fontFileAssetPath.c_str() , fontFileSize);
        fontName_ = fontName;
        
        int errCode = -1;
        errCode = initFont();
        if(errCode < 0){
            return errCode;
        }
        errCode = createFontTextureRes();
        return errCode; 
    }

    int TextRender::initFont(){
        int errCode = stbtt_InitFont(&fontInfo_ , fontData_ , 0);
        fontScale_ = stbtt_ScaleForPixelHeight(&fontInfo_ , FONT_DEFAULT_SIZE);
        Log::i(TAG , "fontScale =  %f" , fontScale_);
        return errCode;
    }

    void TextRender::layoutText(std::wstring &content, 
                SdfTextRenderCommand &renderCmd,
                TextRenderOutInfo &outInfo,
                std::vector<float> &buf){
        TextPaint paint = renderCmd.paint_;
        Rect limitRect = renderCmd.limitRect_;
        
        Rect &outRect = outInfo.outRect;
        outRect.left = limitRect.left;
        outRect.top = limitRect.top;
        outRect.width = 0.0f;
        outRect.height =(FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;

        float maxBaselineY = 0.0f;
        float lineWidth = 0.0f;
        bool isFirstLine = true;

        float x = limitRect.left;
        float y = limitRect.top;
        float depthValue = renderCmd.engine_->getAndChangeDepthValue();

        int index = 0;
        int realRenderCharCount = 0;
        const int size = content.length();

        while(index < size){
            wchar_t ch = content[index];
            auto charInfoPtr = findCharInfo(ch , index);
            if(isFirstLine){
                float bearingY = charInfoPtr->bearingY * paint.textSizeScale;
                if(maxBaselineY <= bearingY){
                    maxBaselineY = bearingY;
                }
            }

            float charRealWidth = (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;

            const int adjustPadding = 16;
            if(x + charRealWidth <= limitRect.getRight() + adjustPadding 
                && ch != L'\n'){
                renderCmd.putVertexDataToBuf(buf , index , x , y ,depthValue,
                    charInfoPtr , paint);
                x += charRealWidth;
                lineWidth += charRealWidth;
                if(outRect.width < lineWidth){
                    outRect.width = lineWidth;
                }
                index++;
                realRenderCharCount = index;
            }else{// change a new line
                isFirstLine = false;

                x = limitRect.left;
                y -= (FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;
                if(y - maxBaselineY < limitRect.getBottom()){
                    break;
                }

                outRect.height += (FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;
                lineWidth = 0.0f;
                
                if(ch == L'\n'){
                    index++;
                }
            }
        }//end while

        outInfo.renderTextSize = index;

        float translateX = limitRect.left - outRect.left;
        float translateY = -maxBaselineY;
        
        switch(paint.textGravity){
            case TopLeft:
                break;
            case TopCenter:
                translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
                outRect.left += translateX;
                break;
            case TopRight:
                translateX += (limitRect.width - outRect.width);
                outRect.left += translateX;
                break;
            case BottomLeft:
                translateY -= (limitRect.height - outRect.height);
                outRect.top += translateY + maxBaselineY;
                break;
            case BottomCenter:
                translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
                outRect.left += translateX;
                translateY -= (limitRect.height - outRect.height);
                outRect.top += translateY + maxBaselineY;
                break;
            case BottomRight:
                translateX += (limitRect.width - outRect.width);
                outRect.left += translateX;
                translateY -= (limitRect.height - outRect.height);
                outRect.top += translateY + maxBaselineY;
                break;
            case CenterLeft:
                translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
                outRect.top += translateY + maxBaselineY;
                break;
            case CenterRight:
                translateX += (limitRect.width - outRect.width);
                outRect.left += translateX;
                translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
                outRect.top += translateY + maxBaselineY;
                break;
            case Center:
                translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
                outRect.left += translateX;
                translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
                outRect.top += translateY + maxBaselineY;
                break;
        }//end switch

        for(int i = 0 ; i < realRenderCharCount ;i++){
            renderCmd.updateVertexPositionData(buf , i , translateX , translateY);
        }//end for i
    }
}

