#include "render/texture.h"
#include "glheader.h"
#include "resource/asset_manager.h"

namespace purple{
    GLint convertChanelToInternalFormat(int format){
        GLint  internalFormat = GL_RGBA;
        switch (format) {
            case GL_RED:
                internalFormat = GL_R8;
                break;
            case GL_RGB:
                internalFormat = GL_RGB;
                break;
        }//end switch
        return internalFormat;
    }

    std::shared_ptr<TextureManager> TextureManager::getInstance(){
        static std::shared_ptr<TextureManager> instance_;
        if(instance_ == nullptr){
            instance_ = std::make_shared<TextureManager>();
        }
        return instance_;
    }

    void TextureManager::freeTexture(TextureInfo &info){
        if(textureBank_.find(info.name) == textureBank_.end()){
            return;
        }
        auto texInfoPtr = textureBank_[info.name];
        // Logi("texture_manager" , "texture del %s" , (texInfoPtr->name).c_str());
        if(texInfoPtr != nullptr){
            glDeleteTextures(1 , &(texInfoPtr->textureId));
        }
        textureBank_.erase(info.name);
        Log::i("texture_manager" , "texture  %s is free!" , info.name.c_str());
    }

    void TextureManager::clear(){
        for(auto pair : textureBank_){
            auto texInfoPtr= pair.second;
            Log::i("texture_manager" , "texture del %s" , (texInfoPtr->name).c_str());

            if(texInfoPtr != nullptr){
                glDeleteTextures(1 , &(texInfoPtr->textureId));
                
                //visual texture delete
                if(texInfoPtr->category == TextureCategory::VIRTUAL_TEX){
                    if(texInfoPtr->renderBufferId != 0){
                        GLuint ids[] = {texInfoPtr->renderBufferId};
                        glDeleteRenderbuffers(1 , ids);
                    }
                    
                    if(texInfoPtr->framebufferId != 0){
                        GLuint ids[] = {texInfoPtr->framebufferId};
                        glDeleteFramebuffers(1 , ids);
                    }
                }//end if
            }//end if
        }//end for each
        textureBank_.clear();
        Log::i("texture_manager" , "texture manager clear");
    }

    std::unique_ptr<uint8_t> TextureManager::readTextureFile(std::string &path,
            bool needFlip, int &format,int &width , int &height){
        TextureFileConfig fileConfig;
        std::unique_ptr<uint8_t> data = AssetManager::getInstance()
            ->readTextureFile(path ,fileConfig,needFlip);

        format = GL_RGBA;
        if(fileConfig.channel == TEXTURE_FILE_CHANNEL_RGB){
            format = GL_RGB;
        }else if(fileConfig.channel == TEXTURE_FILE_CHANNEL_ARGB){
            format = GL_RGBA;
        }else if(fileConfig.channel == TEXTURE_FILE_CHANNEL_RGBA){
            format = GL_RGBA;
        }else if(fileConfig.channel == TEXTURE_FILE_CHANNEL_R){
            format = GL_RED;
        }

        width = fileConfig.width;
        height = fileConfig.height;

        return data;
    }

    //载入纹理数组
    std::shared_ptr<TextureInfo> TextureManager::loadTextureArray(
        std::vector<std::string> &textureFiles , bool needFlip){
        if(textureFiles.empty()){
            return nullptr;
        }

        unsigned int textureId = -1;
        glGenTextures(1 , &textureId);
        if(textureId <= 0 ){
            return nullptr;
        }

        std::string firstFilePath = textureFiles[0];
        int format = GL_RGBA;
        int texWidth = 0;
        int texHeight = 0;
        std::unique_ptr<uint8_t> data = readTextureFile(firstFilePath , 
            needFlip , format, 
            texWidth , texHeight);
        
        glBindTexture(GL_TEXTURE_2D_ARRAY , textureId);
        // glPixelStorei(GL_UNPACK_ALIGNMENT , 4);
        glTexParameterf(GL_TEXTURE_2D_ARRAY , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D_ARRAY , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    convertChanelToInternalFormat(format),
                    texWidth,
            texHeight, textureFiles.size(),
            0,format, GL_UNSIGNED_BYTE , nullptr);
        // Log::i("debug" , "3333load texture before %d" , glGetError());

        for(int i = 0 ; i < textureFiles.size() ;i++){
            std::unique_ptr<uint8_t> pTexData = nullptr;
            int format = TEXTURE_FILE_CHANNEL_UNKNOW;
            int texWidth = 0;
            int texHeight = 0;
            pTexData = readTextureFile(textureFiles[i] , 
                needFlip , format, 
                texWidth , texHeight);
            
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY , 0 , 0 , 0, i, texWidth,
                texHeight , 1 , format, GL_UNSIGNED_BYTE, pTexData.get());
        }//end for i

        glBindTexture(GL_TEXTURE_2D_ARRAY , 0);
    //  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        auto textureInfo = std::make_shared<TextureInfo>();
        textureInfo->name = firstFilePath;
        textureInfo->textureId = textureId;
        textureInfo->type = TextureType::TEXTURE_2D_ARRAY;
        textureInfo->width = texWidth;
        textureInfo->height = texHeight;
        textureInfo->depth = textureFiles.size();
        textureInfo->format = format;

        textureBank_[textureInfo->name] = textureInfo;
        return textureInfo;
    }

    //从像素数据 生成纹理
    std::shared_ptr<TextureInfo> TextureManager::loadTextureFromPixelData(std::string texName 
            , uint8_t *pixelData 
            , int channelFormat 
            , int width 
            , int height){
        int format = channelFormat;

        unsigned int tId = -1;
        glGenTextures(1 , &tId);
        if(tId <= 0 ){
            return nullptr;
        }

        glBindTexture(GL_TEXTURE_2D , tId);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, 
            convertChanelToInternalFormat(format),
            width, 
            height, 0, 
            format, GL_UNSIGNED_BYTE, pixelData);
        glBindTexture(GL_TEXTURE_2D , 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        auto textureInfo = std::make_shared<TextureInfo>();
        textureInfo->name = texName;
        textureInfo->textureId = tId;
        textureInfo->width = width;
        textureInfo->height = height;
        textureInfo->format = format;

        //add pool
        textureBank_[textureInfo->name] = textureInfo;
        
        Log::i(TAG , "load texture id : %d , width : %d , height : %d" , 
            textureInfo->textureId,
            textureInfo->width,
            textureInfo->height);
        
        return textureInfo;
    }

    //将纹理传送至GPU
    std::shared_ptr<TextureInfo> TextureManager::loadTexture(std::string textureFilePath , 
        bool needFlip){
        Log::i(TAG , "load texture %s" , textureFilePath.c_str());
        
        int format = GL_RGBA;
        int texWidth;
        int texHeight;
        auto data = readTextureFile(textureFilePath , needFlip , format , 
            texWidth, texHeight);    
        
       return loadTextureFromPixelData(textureFilePath , data.get() , format , texWidth , texHeight);
    }

    std::shared_ptr<TextureInfo> TextureManager::acquireTexture(std::string textureFilePath , bool needFlip){
        if(textureBank_.find(textureFilePath) != textureBank_.end()){
            return textureBank_[textureFilePath];
        }
        return loadTexture(textureFilePath , needFlip);
    }

    std::shared_ptr<TextureInfo> TextureManager::createEmptyTexture(std::string texName, 
            int width , int height , int format){
        unsigned int tId = -1;
        glGenTextures(1 , &tId);
        if(tId <= 0 ){
            return nullptr;
        }

        glBindTexture(GL_TEXTURE_2D , tId);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, 
            format,
            width, 
            height, 0, 
            format, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D , 0);
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        auto textureInfo = std::make_shared<TextureInfo>();
        textureInfo->name = texName;
        textureInfo->textureId = tId;
        textureInfo->width = width;
        textureInfo->height = height;
        textureInfo->format = format;

        //add pool
        textureBank_[textureInfo->name] = textureInfo;
        
        Log::i(TAG , "load texture id : %d , width : %d , height : %d" , 
            textureInfo->textureId,
            textureInfo->width,
            textureInfo->height);
        
        return textureInfo;
    }

    std::string TextureManager::allTextureInfos(){
        std::string infoString = std::to_string(textureBank_.size()) +" [";

        for(auto &p : textureBank_){
            infoString += p.first;
            infoString += p.second->textureId;
            infoString += " ";
        }//end for each

        infoString += "]";
        return infoString;
    }
}



