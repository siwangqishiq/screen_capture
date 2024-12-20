#include "ui/img.h"
#include "purple.h"
#include <algorithm>
#include "utils.h"

namespace purple{
    
    std::shared_ptr<TextureImage> ImageSource::fromAsset(std::string assetPath){
        return BuildImageByAsset(assetPath);
    }
        
    std::shared_ptr<TextureImage> ImageSource::fromFile(std::string path){
        return nullptr;
    }

    Img::~Img(){
    }

    void Img::onMeasure(MeasureSpecMode widthSpecMode, 
                        int widthValue, 
                        MeasureSpecMode heightSpecMode,
                        int heightValue){
        if(widthSpecMode == MeasureSpecMode::Exactly){
            setWidth(widthValue);
        }else if(widthSpecMode == MeasureSpecMode::Atmost){
            int contentWidth = 0;
            if(textureImage_ != nullptr){
                contentWidth = textureImage_->getWidth();
            }
            const int realWidth = std::min(paddingLeft_ + paddingRight_ + contentWidth , widthValue);
            setWidth(realWidth);
        }else if(widthSpecMode == MeasureSpecMode::Unset){
            setWidth(0);
        }

        if(heightSpecMode == MeasureSpecMode::Exactly){
            setHeight(heightValue);
        }else if(heightSpecMode == MeasureSpecMode::Atmost){
            int contentHeight = 0;
            if(textureImage_ != nullptr){
                contentHeight = textureImage_->getHeight();
            }
            const int realHeight = std::min(paddingTop_ + paddingBottom_ + contentHeight , heightValue);
            setHeight(realHeight);
        }else if(widthSpecMode == MeasureSpecMode::Unset){
            setHeight(0);
        }

        // Log::i("ui","Image Widget measue size %d , %d   %s" , width_ , height_ , this->id.c_str());
    }
            
    void Img::onRender(){
        this->Widget::onRender();

        //render image
        if(textureImage_ == nullptr 
            || textureImage_->getWidth() == 0 
            || textureImage_->getHeight() == 0){
            return;
        }

        Rect srcRect = textureImage_->getRect();

        const int viewWidth = getWidth() - paddingRight_ - paddingLeft_;
        const int viewHeight = getHeight() - paddingBottom_ - paddingTop_;
        Rect viewRect(left + paddingLeft_, 
                            top - paddingTop_, 
                            viewWidth, viewHeight);
        // Log::i("debug" , "render text id in img %d", textureImage_->getTextureId());
        Rect dstRect;
        switch(scaleMode_){
            case ImgScale::Mode::FitCenter:
                dstRect = findFitScaleDstRect(srcRect,viewRect);
                break;
            case ImgScale::Mode::FitTop:
                dstRect = findFitTopDstRect(srcRect,viewRect);
                break;
            case ImgScale::Mode::FitBottom:
                dstRect = findFitBottomDstRect(srcRect,viewRect);
                break;
            case ImgScale::Mode::FitXY:
                dstRect = viewRect;
                break;
            case ImgScale::Mode::Center:
                dstRect = findCenterDstRect(srcRect,viewRect);
                break;
            case ImgScale::Mode::CenterCrop:
                dstRect = findCenterCropDstRect(srcRect,viewRect);
                break;
            case ImgScale::Mode::CenterInside:
                dstRect = findCenterInsideDstRect(srcRect,viewRect);
                break;
            default:
                break;
        }//end switch


        auto render = purple::Engine::getRenderEngine();
        auto speiteBatch = render->getSpriteBatch();
        speiteBatch->begin();
        speiteBatch->renderImage(*textureImage_ , srcRect , dstRect);
        speiteBatch->end();
    }

    Rect Img::findFitTopDstRect(Rect &srcRect,Rect &viewRect){
        Rect dstRect = findFitScaleDstRect(srcRect,viewRect);
        dstRect.top = viewRect.top;
        return dstRect;
    }

    Rect Img::findFitBottomDstRect(Rect &srcRect,Rect &viewRect){
        Rect dstRect = findFitScaleDstRect(srcRect,viewRect);
        dstRect.top = viewRect.top - viewRect.height + dstRect.height;
        return dstRect;
    }

    Rect Img::findFitScaleDstRect(Rect &srcRect,Rect &viewRect){
        const float ratio = srcRect.width / srcRect.height;
        Rect dstRect;
        if(viewRect.width >= viewRect.height){
            dstRect.height = viewRect.height;
            dstRect.width = dstRect.height * ratio;
            if(dstRect.width > viewRect.width){
                dstRect.width = viewRect.width;
                dstRect.height = dstRect.width / ratio;
                dstRect.left = viewRect.left;
                dstRect.top = viewRect.top - viewRect.height / 2.0f + dstRect.height / 2.0f;
            }else{
                dstRect.left = viewRect.left + viewRect.width / 2.0f - dstRect.width / 2.0f;
                dstRect.top = viewRect.top;
            }
        }else{
            dstRect.width = viewRect.width;
            dstRect.height = viewRect.width/ ratio;
            if(dstRect.height > viewRect.height){
                dstRect.height = viewRect.height;
                dstRect.width = dstRect.height * ratio;
                dstRect.left = viewRect.left + viewRect.width / 2.0f - dstRect.width / 2.0f;
                dstRect.top = viewRect.top;
            }else{
                dstRect.left = viewRect.left;
                dstRect.top = viewRect.top - viewRect.height / 2.0f + dstRect.height / 2.0f;
            }
        }
        return dstRect;
    }

    Rect Img::findCenterDstRect(Rect &srcRect,Rect &viewRect){
        Rect dstRect;
        if(srcRect.width <= viewRect.width){
            dstRect.width = srcRect.width;
            dstRect.left = viewRect.left + viewRect.width / 2.0f - dstRect.width / 2.0f;
        } else { //原图大于view 需要截断
            dstRect.width = viewRect.width;
            dstRect.left = viewRect.left;

            srcRect.left = srcRect.left + (srcRect.width - viewRect.width) / 2.0f;
            srcRect.width = dstRect.width;
        }

        if(srcRect.height <=viewRect.height){
            dstRect.height = srcRect.height;
            dstRect.top = viewRect.top - viewRect.height / 2.0f + dstRect.height / 2.0f;
        }else{
            dstRect.height = viewRect.height;
            dstRect.top = viewRect.top;

            srcRect.top = srcRect.top - (srcRect.height - viewRect.height)/2.0f;
            srcRect.height = viewRect.height;
        }
        return dstRect;
    }

    Rect Img::findCenterModeRect(Rect &srcRect,Rect &viewRect,bool isCrop){
        Rect dstRect;
        dstRect.left = viewRect.left;
        dstRect.top = viewRect.top;
        dstRect.width = srcRect.width;
        dstRect.height = srcRect.height;

        auto dstCenter = dstRect.center();
        auto viewCenter = viewRect.center();
        float tranX = dstCenter.x - viewCenter.x;
        float tranY = dstCenter.y - viewCenter.y;

        dstRect.left -= tranX;
        dstRect.top -= tranY;
        
        // is in view quit
        // if(dstRect.left >= viewRect.left && dstRect.top <= viewRect.top){
        //     // std::cout << "this src is in view" << std::endl;
        //     return dstRect;
        // }

        const float scaleWidth = viewRect.width / dstRect.width;
        Point pw{dstRect.left , dstRect.top};
        ScaleWithPoint(pw , scaleWidth , viewCenter);
        int wLeft = pw.x;
        int wTop = pw.y;

        const float scaleHeight = viewRect.height / dstRect.height;
        Point ph{dstRect.left , dstRect.top};
        ScaleWithPoint(ph , scaleHeight , viewCenter);
        int hLeft = ph.x;
        int hTop = ph.y;
        
        float scale = 1.0f;

        const int viewLeft = viewRect.left;
        const int viewTop = viewRect.top;

        if(isCrop){
            dstRect= viewRect;
            // std::cout << "scale Width " << scaleWidth  
            //     << "  scale Height: " << scaleHeight << std::endl;
            // std::cout << "Width wleft " << wLeft << " wTop: " << wTop  
            //     << " Height hLeft: " << hLeft << "  hTop: " << hTop << std::endl;

            if(wLeft >= viewLeft && wTop < viewTop){
                scale = scaleHeight;
            }else if(hLeft > viewRect.left && hTop >= viewTop) {
                scale = scaleWidth;
            }
            // std::cout << "use scale = " << scale << std::endl;

            Rect copySrcRect;
            copySrcRect.width = scale * srcRect.width;
            copySrcRect.height = scale * srcRect.height;
            copySrcRect.left = 0.0f;
            copySrcRect.top = copySrcRect.height;

            float deltaW = (copySrcRect.width - dstRect.width) / 2.0f;
            float deltaH = (copySrcRect.height - dstRect.height) / 2.0f;
            copySrcRect.left = copySrcRect.left + deltaW;
            copySrcRect.top = copySrcRect.top - deltaH;
            copySrcRect.width = copySrcRect.width - 2.0f * deltaW;
            copySrcRect.height = copySrcRect.height - 2.0f * deltaH;

            //  std::cout << "copySrcRect : " << copySrcRect.left << ","
            //     << copySrcRect.top << ", " << copySrcRect.width << " , "
            //     << copySrcRect.height << std::endl;

            // Point srcPoint{copySrcRect.left, copySrcRect.top};
            const float inverseScale = 1.0f / scale;
            // ScaleWithPoint(srcPoint , inverseScale , dstRect.center());
            srcRect.left = copySrcRect.left * inverseScale;
            srcRect.top = copySrcRect.top * inverseScale;
            srcRect.width = copySrcRect.width * inverseScale;
            srcRect.height = copySrcRect.height * inverseScale;
            // std::cout << "srcRect : " << srcRect.left << ","
            //     << srcRect.top << ", " << srcRect.width << " , "
            //     << srcRect.height << std::endl;
        
        }else{
            if(wLeft >= viewLeft && wTop < viewTop){
                scale = scaleWidth;
                dstRect.left = wLeft;
                dstRect.top = wTop;
            }else if(hLeft > viewRect.left && hTop >= viewTop){
                scale = scaleHeight;
                dstRect.left = hLeft;
                dstRect.top = hTop;
            }else{
                // std::cout << "set scale 0" << std::endl;
                scale = 0.0f;
            }

            dstRect.width = dstRect.width * scale;
            dstRect.height = dstRect.height * scale;
        }


        // //do scale and src crop
        // const float srcWidth = srcRect.width;
        // const float srcHeight = srcRect.height;

        // const float scaleW = viewRect.width / srcWidth;
        // const float scaleH = viewRect.height / srcHeight;
        // // const float scale = isCrop?std::max(scaleW , scaleH):std::min(scaleW , scaleH);
        // const float scale = isCrop?std::max(scaleH , scaleW):std::min(scaleW , scaleH);
        
        // //scale with center point
        // dstRect.width = scale * srcWidth;
        // dstRect.heig
        // if(dstRect.top > viewRect.top){
        //     srcRect.top = srcRect.top - (dstRect.top  - viewRect.top) / (scale);
        //     srcRect.heightht = scale * srcHeight;

        // dstRect.left = scale * dstRect.left + viewCenter.x - viewCenter.x * scale;
        // dstRect.top = scale * dstRect.top + viewCenter.y - viewCenter.y * scale;

        // // crop src  = viewRect.height / scale;

        //     dstRect.top = viewRect.top;
        //     dstRect.height = viewRect.height;
        // }else if(dstRect.left < viewRect.left){
        //     srcRect.left = srcRect.left + (viewRect.left - dstRect.left) / (scale);
        //     srcRect.width = viewRect.width / scale;

        //     dstRect.left = viewRect.left;
        //     dstRect.width = viewRect.height;
        // }
        return dstRect;
    }

    Rect Img::findCenterCropDstRect(Rect &srcRect,Rect &viewRect){
        return findCenterModeRect(srcRect , viewRect , true);
    }

    Rect Img::findCenterInsideDstRect(Rect &srcRect,Rect &viewRect){
        return findCenterModeRect(srcRect , viewRect , false);
    }
}