/**
 * texture  
 * 
 */

#pragma once

#include <memory>
#include <unordered_map>
#include "log.h"
#include <vector>
#include "glheader.h"

namespace purple{
        
    enum TextureType{
        TEXTURE_2D,
        TEXTURE_2D_ARRAY
    };

    enum TextureCategory{
        NORMAL_TEX, //正常纹理
        VIRTUAL_TEX //虚拟纹理
    };

    struct TextureInfo{
        std::string name;//纹理名称
        unsigned int textureId = -1;//纹理ID
        int width;
        int height;
        int depth;
        int format;

        TextureType type = TEXTURE_2D;
        
        TextureCategory category = NORMAL_TEX; //纹理状态
        unsigned int renderBufferId = 0;
        unsigned int framebufferId = 0;
    };

    GLint convertChanelToInternalFormat(int channelFormat);

    //纹理管理
    class TextureManager{
    public:
        static std::shared_ptr<TextureManager> getInstance();

        const std::string TAG = "TextureManager";

        TextureManager(){
            Log::i(TAG , "TextureManager construct");
        }

        ~TextureManager(){
            Log::i(TAG , "TextureManager deconstruct");
            clear();
        }
        
        void clear();

        void freeTexture(TextureInfo &info);

        std::shared_ptr<TextureInfo> acquireTexture(std::string textureFilePath , bool needFlip = false);

        std::shared_ptr<TextureInfo> loadTexture(std::string textureFilePath ,bool needFlip = false);

        std::shared_ptr<TextureInfo> loadTextureFromPixelData(std::string texName ,uint8_t *pixelData , int channelFormat , int width , int height);

        std::shared_ptr<TextureInfo> createEmptyTexture(std::string texName, 
            int width , int height , int format);

        std::string allTextureInfos();

        std::unique_ptr<uint8_t> readTextureFile(std::string &path ,bool needFlip
            , int &format , int &width , int &height);

        std::shared_ptr<TextureInfo> loadTextureArray(
            std::vector<std::string> &textureFiles , 
            bool needFlip = false);

        std::shared_ptr<TextureInfo> createEmptyTexture2dArray(
            std::string texName, 
            int width , int height , int depth, int format);

        int updateTexture2dArrayData(std::shared_ptr<TextureInfo> textureInfo,
            int startX , 
            int startY , 
            int startZ , 
            int w, 
            int h,
            int depthSize ,
            uint8_t *updateData);
    private:
        static std::shared_ptr<TextureManager> instance_;

        std::unordered_map<std::string , std::shared_ptr<TextureInfo>> textureBank_;
    };

}


