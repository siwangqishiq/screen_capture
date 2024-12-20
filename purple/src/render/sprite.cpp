#include "render/sprite.h"
#include "render/texture.h"
#include "render/common.h"

namespace purple{
        
    std::shared_ptr<TextureImage> BuildImageByAsset(std::string path){
        std::shared_ptr<TextureInfo> info = TextureManager::getInstance()->acquireTexture(path , true);
        return std::make_shared<TextureImage>(info);
    }

    std::shared_ptr<TextureImage> BuildImageByPixlData(uint8_t *pixelData , int width , int height, int format){
        std::string texName = std::string("pixel_image_") 
            + std::to_string(width)+std::string("x")
            + std::to_string(height);
         std::shared_ptr<TextureInfo> info = TextureManager::getInstance()->loadTextureFromPixelData(texName , pixelData , format , width , height);
         return std::make_shared<TextureImage>(info);
    }

    std::shared_ptr<TextureImage> BuildEmptyImage(int width , int height , int format){
        static int emptyTextureCount = 0;
        emptyTextureCount++;

        std::string name = std::string("empty_image") 
            + std::to_string(emptyTextureCount) 
            + std::to_string(width)+std::string("x")
            + std::to_string(height);
        auto info = TextureManager::getInstance()->createEmptyTexture(name , width , height , format);
        return std::make_shared<TextureImage>(info);
    }

    std::shared_ptr<TextureImageRegion> TextureImage::createImageRegion(float offsetX, float offsetY, float regionWidth , float regionHeight){
        return std::make_shared<TextureImageRegion>(getTextureId(),
            offsetX , offsetY , regionWidth , regionHeight , getWidth() , getHeight());
    }

    TextureImage::TextureImage(std::shared_ptr<TextureInfo> textureInfo){
        textureInfo_ = textureInfo;
    }

    Rect TextureImage::getRect(){
        Rect rect;
        rect.left = 0.0f;
        rect.top = getHeight();
        rect.width = getWidth();
        rect.height = getHeight();
        return rect;
    }

    int TextureImage::getWidth(){
        return textureInfo_ != nullptr? textureInfo_->width : 0;
    }

    int TextureImage::getHeight(){
        return textureInfo_ != nullptr? textureInfo_->height : 0;
    }

    int TextureImage::getPixFormat(){
        return textureInfo_ != nullptr? textureInfo_->format : 0;
    }

    long TextureImage::textireSize(){
        return getWidth() * getHeight() * 4; //统一按照4字节计算
    }

    void TextureImage::dispose(){
        if(textureInfo_ == nullptr){
            return;
        }
        
        TextureManager::getInstance()->freeTexture(*textureInfo_);
    }

    void TextureImage::updateTextureData(uint8_t *pData){

        std::cout << "updateTextureData " << glGetError() << std::endl;
        glBindTexture(GL_TEXTURE_2D , getTextureId());
        glTexSubImage2D(GL_TEXTURE_2D , 
            0 , 0,  0,
            getWidth() , 
            getHeight(), 
            getPixFormat(), 
            GL_UNSIGNED_BYTE, pData);
        glBindTexture(GL_TEXTURE_2D , 0);

        std::cout << "updateTextureData after " << glGetError() << std::endl;
    }

    unsigned int TextureImage::getTextureId(){
        return textureInfo_ != nullptr? textureInfo_->textureId : 0;
    }

    Point TextureImageRegion::getOffset(){
        return Point(offsetX_ , offsetY_);
    }

    float TextureImageRegion::getWidth(){
        return width_;
    }

    float TextureImageRegion::getHeight(){
        return height_;
    }

    float TextureImageRegion::getTexWidth(){
        return texWidth_;
    }

    float TextureImageRegion::getTexHeight(){
        return texHeight_;
    }

    unsigned int TextureImageRegion::getTextureId(){
        return textureId_;
    }
}
