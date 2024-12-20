#include "render/cmd/cmd_text.h"
#include "utils.h"

namespace purple{
    float TextRenderCommand::findCharMaxHeight(std::wstring &text , TextPaint &paint){
        auto textRenderHelper = engine_->textRenderHelper_;
        float maxHeight = 0.0f;
        for(int i = 0 ; i < text.length() ;i++){
            wchar_t ch = text[i];
            auto charInfoPtr = textRenderHelper->findCharInfo(ch);
            if(charInfoPtr == nullptr){
                continue;
            }
            
            float realHeight = charInfoPtr->height * paint.textSizeScale;
            if(maxHeight < realHeight) {
                maxHeight = realHeight;
            }
        }//end for i
        return maxHeight;
    }

    float TextRenderCommand::calOffsetY(std::shared_ptr<CharInfo> charInfo , float scale){
        if(TextRenderHelper::isSymbol(charInfo->value)){
            return 0.0f;
        }else{
            float charRealHeight = charInfo->height * scale;
            float fontMaxHeight = FONT_DEFAULT_SIZE * scale;
            return fontMaxHeight / 2.0f - charRealHeight / 2.0f;
        }
    }

    float TextRenderCommand::calTextStyleItalicOffset(std::shared_ptr<CharInfo> charInfo , TextPaint &paint){
        // Logi("italic" , "offset %d" , paint.textStyle);
        if(charInfo == nullptr || paint.textStyle == TextStyle::normal){
            return 0.0f;
        }else if(paint.textStyle == TextStyle::italic){
            return (FONT_DEFAULT_SIZE * paint.textSizeScale) / 3.0f;
        }
        return 0.0f;
    }

    unsigned int TextRenderCommand::allocatorVRamForText(int textLength){
        vertexCount_ = vertCountPerChar_ * textLength;
        // Logi("TextRenderCommand" , "vertexcount = %d", vertexCount_);
        int requestSize = vertexCount_ * attrCount_ * sizeof(float);
        int allocateSize = 0;
        // Log::i("command" , "allocator size = %d" , requestSize);
        allocatorVRam(requestSize , allocateSize);
        return vbo_;
    }

    Rect TextRenderCommand::createEmptyWrapRect(Rect &limitRect , TextPaint &paint){
        Rect outRect;
        outRect.left = limitRect.left;
        outRect.top = limitRect.top;
        outRect.width = 0.0f;
        outRect.height =(FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;
        return outRect;
    }

    void TextRenderCommand::putParams(std::wstring text 
            ,float left , float bottom
            ,TextPaint &paint){
        paint_ = paint;
        fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;

        // auto t1 = currentTimeMillis();
        allocatorVRamForText(text.length());
        // Logi("TextRenderCommand" , "allocatorVRamForText costtime : %lld" , (currentTimeMillis() - t1));

        if(vbo_ <= 0){
            return;
        }
        // Logi("command" , "allocator vbo = %d" , vbo_);

        float x = left;
        float y = bottom;

        long long t2 = currentTimeMicro();
        auto textRenderHelper = engine_->textRenderHelper_;
        std::vector<float> buf(vertexCount_ * attrCount_);
        
        float depth = engine_->getAndChangeDepthValue();
        for(int i = 0 ; i < text.length() ;i++){
            wchar_t ch = text[i];
            auto charInfoPtr = textRenderHelper->findCharInfo(ch);
            // Logi("TextRenderCommand" , "%f %f" 
            //     , charInfoPtr->textureCoords[2],charInfoPtr->textureCoords[3]);
            putVertexDataToBuf(buf , i , x , y ,depth, charInfoPtr , paint);
            x += (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;
        }//end for i
        // Logi("TextRenderCommand" , "layout vertex vertexcount : %d , costtime =  %lld" 
        //     , vertexCount_
        //     , (currentTimeMicro() - t2));

        // auto t3 = currentTimeMillis();
        buildGlCommands(buf);
        // Logi("TextRenderCommand" , "buildGlCommands time : %lld" , (currentTimeMillis() - t3));
    }

    void TextRenderCommand::putTextParamsByRectLimit(std::wstring &text , 
                Rect &limitRect, 
                Rect *wrapContentRect,
                TextPaint &paint){
        if(text.empty()){
            if(wrapContentRect != nullptr){ //empty input to fill out rect
                *wrapContentRect = createEmptyWrapRect(limitRect , paint);
            }
            return;
        }

        allocatorVRamForText(text.length());

        paint_ = paint;
        fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;

        // Logi("debug" , "vertexCount = %d , attrCound = %d" , vertexCount_ , attrCount_);
        std::vector<float> buf(vertexCount_ * attrCount_);
        Rect outRect;
        engine_->textRenderHelper_->layoutText(text , this, outRect, buf);
        if(wrapContentRect != nullptr){
            *wrapContentRect = outRect;
        }
        buildGlCommands(buf);
    }
    
    void TextRenderCommand::updateVertexPositionData(std::vector<float> &buf, 
            int index, float translateX,float translateY){
        const int attrPerChar = attrCount_ * vertCountPerChar_;
        int offset = index * attrPerChar;
        
        //v1
        buf[offset + 0] += translateX;
        buf[offset + 1] += translateY;

        //v2
        buf[offset + 6] += translateX;
        buf[offset + 7] += translateY;

        //v3
        buf[offset + 12] += translateX;
        buf[offset + 13] += translateY;
        
        //v4
        buf[offset + 18] += translateX;
        buf[offset + 19] += translateY;

        //v5
        buf[offset + 24] += translateX;
        buf[offset + 25] += translateY;

        //v6
        buf[offset + 30] += translateX;
        buf[offset + 31] += translateY;
    }

    void TextRenderCommand::putVertexDataToBuf(std::vector<float> &buf, 
            int index, 
            float x,
            float y,
            float depthValue,
            std::shared_ptr<CharInfo> charInfoPtr ,
            TextPaint &paint){
        const int attrPerChar = attrCount_ * vertCountPerChar_;
        const float sizeScale = paint_.textSizeScale;
        // Logi("text_render" , "size scale %f" , sizeScale);
        // Logi("text_render" , "x y %f %f" , x , y);

        float charRealWidth = charInfoPtr->width * sizeScale;
        float charRealHeight = charInfoPtr->height * sizeScale;

        float italicOffset = calTextStyleItalicOffset(charInfoPtr , paint_);
        // Log::i("text_render" , "originw : %f  originh: %f , charRealWidth %f , charRealHeight %f  scale: %f" ,
        //     charInfoPtr->width, charInfoPtr->height ,charRealWidth,charRealHeight , paint_.textSizeScale);

        float texLeft = charInfoPtr->textureCoords[0];
        float texTop = charInfoPtr->textureCoords[1];
        float texRight = charInfoPtr->textureCoords[2];
        float texBottom = charInfoPtr->textureCoords[3];
        float texW = charInfoPtr->textureCoords[4];

        // Log::i("text_render" , "l t r b %f %f %f %f" , texLeft , texTop , texRight , texBottom);

        //eg: 一 need a offset in y ax
        float offsetX = 0.0f;
        float offsetY = (charInfoPtr->bearingY - charInfoPtr->height) * sizeScale;

        int offset = index * attrPerChar;

        //v1
        buf[offset + 0] = x + offsetX;
        buf[offset + 1] = y + offsetY;
        buf[offset + 2] = depthValue;
        buf[offset + 3] = texLeft;
        buf[offset + 4] = texBottom;
        buf[offset + 5] = texW;
        
        //v2
        buf[offset + 6] = x + charRealWidth + offsetX;
        buf[offset + 7] = y + offsetY;
        buf[offset + 8] = depthValue;
        buf[offset + 9] = texRight;
        buf[offset + 10] = texBottom;
        buf[offset + 11] = texW;

        //v3
        buf[offset + 12] = x + charRealWidth + offsetX + italicOffset;
        buf[offset + 13] = y + offsetY + charRealHeight;
        buf[offset + 14] = depthValue;
        buf[offset + 15] = texRight;
        buf[offset + 16] = texTop;
        buf[offset + 17] = texW;
        
        //v4
        buf[offset + 18] = x + offsetX;
        buf[offset + 19] = y + offsetY;
        buf[offset + 20] = depthValue;
        buf[offset + 21] = texLeft;
        buf[offset + 22] = texBottom;
        buf[offset + 23] = texW;

        //v5
        buf[offset + 24] = x + charRealWidth + offsetX + italicOffset;
        buf[offset + 25] = y + offsetY + charRealHeight;
        buf[offset + 26] = depthValue;
        buf[offset + 27] = texRight;
        buf[offset + 28] = texTop;
        buf[offset + 29] = texW;

        //v6
        buf[offset + 30] = x + offsetX + italicOffset;
        buf[offset + 31] = y + offsetY + charRealHeight;
        buf[offset + 32] = depthValue;
        buf[offset + 33] = texLeft;
        buf[offset + 34] = texTop;
        buf[offset + 35] = texW;
    }


    void TextRenderCommand::buildGlCommands(std::vector<float> &buf){
        glBindVertexArray(vao_);
        // Logi("cmd" , "vboOffset_ = %d",vboOffset_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ , 
            buf.size() * sizeof(float) , buf.data());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }

    void TextRenderCommand::runCommands(){
        if(fontTextureId_ <= 0){
            return;
        }
        // Logi("TextRenderCommand" , "runCommands");
        
        //打开混合模式 文字有透明度
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
        
        auto shader = engine_->textRenderHelper_->textRenderShader_;
        shader.useShader();
        shader.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader.setUniformVec4("textColor" , paint_.textColor);
        
        glBindVertexArray(vao_);
        // Logi("cmmmand" , "vbo id %d vao id %d" , vbo_ , vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, fontTextureId_);
        shader.setUniformInt("fontTexture" , 0);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }

    //计算文本顶点数据
    void TextRenderCommand::fillTextVertData(std::wstring &text , float left , float bottom,
            TextPaint &paint){
        auto textRenderHelper = engine_->textRenderHelper_;

        float x = left;
        float y = bottom;
        
        vertexCount_ = 6 * text.length();

        for(int i = 0; i < text.size() ;i++) {
            wchar_t ch = text[i];
            auto charInfoPtr = textRenderHelper->findCharInfo(ch);
            if(charInfoPtr == nullptr){
                continue;
            }

            float realWidth = charInfoPtr->width * paint.textSizeScale;
            float realHeight = charInfoPtr->height * paint.textSizeScale;

            x += realWidth + paint.gapSize;
        }//end for i;
    }
}