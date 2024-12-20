#include "render/cmd/cmd_text_sdf.h"
#include "render/texture.h"

namespace purple{
    std::shared_ptr<CharInfo> SdfTextRenderCommand::findCharInfo(wchar_t &ch , int index){
        return textRender_->findCharInfo(ch , index);
    }

    void SdfTextRenderCommand::putParams(
            std::wstring &text ,
            float left ,
            float bottom,
            TextPaint &paint){
        paint_ = paint;

        allocatorVRamForText(text.length());
        if(vbo_ <= 0){
            Log::e(TAG , "SdfTextRenderCommand -> vbo is < 0");
            return;
        }

        float x = left;
        float y = bottom;
        std::vector<float> buf(vertexCount_ * attrCount_);
        
        float depth = engine_->getAndChangeDepthValue();
        for(int i = 0 ; i < text.length() ;i++){
            std::shared_ptr<CharInfo> charInfoPtr = findCharInfo(text[i] , i);
            putVertexDataToBuf(buf , i , x , y ,depth, charInfoPtr , paint);
            x += (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;
            // x += (charInfoPtr->width) * paint.textSizeScale;
        }//end for i
        
        buildGlCommands(buf);                 
    }

    void SdfTextRenderCommand::runCommands(){
        if(textRender_->getFontTextureInfo()->textureId <= 0){
            return;
        }
        
        Shader shader = fetchSdfTextShader();
        shader.useShader();
        shader.setUniformMat3("transMat" , engine_->normalMatrix_);
        shader.setUniformVec4("uTextColor" , paint_.textColor);
        shader.setUniformFloat("uFontSize" , paint_.getTextFontHeight());
        shader.setUniformFloat("uFontWeight" , paint_.fontWeight);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER , vbo_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textRender_->getFontTextureInfo()->textureId);

        shader.setUniformInt("sdfTexture" , 0);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , vertCountPerChar_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_));
        glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , vertCountPerChar_ * sizeof(float) , 
            reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
        glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glBindBuffer(GL_ARRAY_BUFFER , 0);
        glBindVertexArray(0);
    }



    Shader SdfTextRenderCommand::fetchSdfTextShader(){
        return ShaderManager::getInstance()->loadAssetShader(SAHDER_NAME_SDF_TEXT, 
                    "shader/text/render_sdftext_vert.glsl",
                    "shader/text/render_sdftext_frag.glsl");
    }


    int SdfTextRenderCommand::putTextParamsByRectLimit(
                    std::wstring &text , 
                    Rect &limitRect, 
                    TextRenderOutInfo *outInfo,
                    TextPaint &paint){
        // std::cout << " putTextParamsByRectLimit beigin ..." << std::endl;
        if(text.empty()){
            // std::cout << " putTextParamsByRectLimit text is Empty" << std::endl;
            if(outInfo != nullptr){ //empty input to fill out rect
                outInfo->outRect = createEmptyWrapRect(limitRect , paint);
                outInfo->renderTextSize = 0;
            }
            return -1;
        }
        allocatorVRamForText(text.length());
        paint_ = paint;

        // Logi("debug" , "vertexCount = %d , attrCound = %d" , vertexCount_ , attrCount_);
        std::vector<float> buf(vertexCount_ * attrCount_);
        TextRenderOutInfo innerOutInfo;
        textRender_->layoutText(text , *this, innerOutInfo, buf);
        if(outInfo != nullptr){
            *outInfo = innerOutInfo;
        }
        buildGlCommands(buf);

        return 0;
    }
}