/**
 * @file render.hpp
 * @author panyi
 * @brief 
 * @date 2022-11-17
 * 
 * renderer
 * 
 * 
 */
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "glm/matrix.hpp"
#include "render/shader.h"
#include <unordered_map>
#include "log.h"
#include <functional>
#include "render/common.h"

namespace purple{
    class RenderCommand;
    class TextRenderHelper;
    class TextRenderCommand;
    class ShaderRenderCommand;
    class ShapeRenderCommand;
    class VRamManager;

    class ShapeBatch;
    class SpriteBatch;
    class TextureInfo;
    class TextRender;
    class TextRenderOutInfo;

    class RenderEngine{
    public:
        const std::string TAG = "RenderEngine";

        RenderEngine(){
            Log::i("RenderEngine" , "Render Engine con");
        }

        ~RenderEngine(){
            Log::i("RenderEngine" , "~RenderEngine Engine deconstruct");
        }

        void init();

        void clearRenderCommands();

        void submitRenderCommand(std::shared_ptr<RenderCommand> cmd);

        void free();

        void onScreenResize();

        std::shared_ptr<TextRenderHelper> textRenderHelper_;

        //归一化变换矩阵
        glm::mat3 normalMatrix_;
        
        //单位矩阵
        glm::mat4 tmpTransMatrix_;

        //==============render api ================

        //自定义带纹理的shader
        void renderTextureShader(
                Shader &shader , 
                Rect &showRect , 
                unsigned int textureId,
                std::function<void(void)> preRenderCallback);

        //弧线
        void renderArc(float cx , float cy , float radius , 
                float startAngle , float endAngle ,
                bool isReverse, Paint &paint);

        //画单色直线
        void renderLines(std::vector<float> &points , Paint &paint);

        //绘制更加平滑的直线
        void renderSmoothLines(std::vector<float> &points , Paint &paint);

        //三角形绘制
        void renderTriangle(
                        float p1x,  float p1y, 
                        float p2x , float p2y,
                        float p3x , float p3y, Paint &paint){
            renderTriangle(p1x , p1y , p2x , p2y , p3x , p3y , tmpTransMatrix_ , paint);
        }

        //三角形绘制
        void renderTriangle(
                            float p1x,  float p1y, 
                            float p2x , float p2y,
                            float p3x , float p3y,
                            glm::mat4 &transMat, Paint &paint);



        //三角形绘制
        void renderTriangles(std::vector<float> &data ,glm::mat4 &transMat, Paint &paint);

        //绘制单独的一个矩形
        void renderRect(Rect &rect , glm::mat4 &transMat , Paint &paint);

        //绘制文字
        void renderText(
            std::wstring &text , 
            float left , 
            float bottom , 
            TextPaint &paint
        );

        //绘制文字
        void renderText(const wchar_t *text , 
            float left , float bottom , TextPaint &paint){
            auto str = std::wstring(text);
            renderText(str, left , bottom , paint);
        }

        //绘制文字  但是将文字限定在一个矩形框内 放不下的文字 直接舍弃 
        void renderTextWithRect(std::wstring &text , 
                Rect &showRect , 
                TextPaint &paint , 
                Rect *wrapContentRect);//
        
        //绘制文字  但是将文字限定在一个矩形框内 放不下的文字 直接舍弃 
        void renderTextWithRect(std::wstring &&text , 
                Rect &showRect , 
                TextPaint &paint , 
                Rect *wrapContentRect){
            renderTextWithRect(text , showRect , paint ,wrapContentRect);
        }
        
        void renderTextWithRect(const wchar_t *text , 
                Rect &showRect , 
                TextPaint &paint ,
                Rect *wrapContentRect){
            auto str = std::wstring(text);
            renderTextWithRect(str , showRect , paint ,wrapContentRect);
        }
        
        //在指定矩形区域内绘制自定义shader
        void renderShader(Shader &shader , Rect &showRect , 
                std::function<void(void)> preRenderCallback);

        //创建虚拟纹理 
        std::shared_ptr<TextureInfo> buildVirtualTexture(
                std::string texName,
                int texWidth , 
                int texHeight ,
                std::function<void(int , int)> renderFn);

        //更新虚拟纹理 
        void updateVirtualTexture(std::shared_ptr<TextureInfo> texInfo ,
                std::function<void(int , int)> renderFn);

        std::shared_ptr<ShapeBatch> getShapeBatch();

        std::shared_ptr<SpriteBatch> getSpriteBatch();

        std::shared_ptr<TextRender> getTextRender(){
            return getTextRenderByName(DEFAULT_TEXT_RENDER_NAME);
        }

        //载入字体
        bool loadTextFontRes(std::string fontName, std::string fontPath);

        float getAndChangeDepthValue();
        
        void resetDepth();


        //绘制文字
        void renderTextV2(
            const wchar_t *text , 
            float left , 
            float bottom , 
            TextPaint &paint);
        
        //rende text new sdf algorithm
        void renderTextV2(
            std::wstring &text , 
            float left , 
            float bottom , 
            TextPaint &paint);

        //绘制文字  但是将文字限定在一个矩形框内 放不下的文字 直接舍弃 
        void renderTextWithRectV2(std::wstring &text , 
                Rect &showRect , 
                TextPaint &paint , 
                TextRenderOutInfo *outInfo);//
        
        //绘制文字  但是将文字限定在一个矩形框内 放不下的文字 直接舍弃 
        void renderTextWithRectV2(std::wstring &&text , 
                Rect &showRect , 
                TextPaint &paint , 
                TextRenderOutInfo *outInfo){
            renderTextWithRectV2(text , showRect , paint ,outInfo);
        }
        
        void renderTextWithRectV2(const wchar_t *text , 
                Rect &showRect , 
                TextPaint &paint ,
                TextRenderOutInfo *outInfo){
            std::wstring textcopy = text;
            renderTextWithRectV2(textcopy , showRect , paint ,outInfo);
        }

        std::shared_ptr<VRamManager> vramManager_;
    private:
        std::vector<std::shared_ptr<RenderCommand>> renderCommandList_;

        void loadTextRenderResource();
        
        void loadShapeShader();

        void resetNormalMat(float w , float h);

        std::shared_ptr<TextRender> getTextRenderByName(std::string name);

        bool loadTextRender(std::string assetFontFile);

        int textCommandIndex;
        std::vector<std::shared_ptr<TextRenderCommand>> textCommandPool;

        void fillLinesFromRect(Rect &rect ,glm::mat4 &transMat, std::vector<float> &buf);

        //fetch new render command
        std::shared_ptr<TextRenderCommand> fetchTextRenderCommand(RenderEngine *engine);
        std::shared_ptr<ShaderRenderCommand> fetchShaderRenderCommand(RenderEngine *engine);
        std::shared_ptr<ShapeRenderCommand> fetchShaderShapeRenderCommand(RenderEngine *engine);


        //batch render
        std::shared_ptr<ShapeBatch> shapeBatch_;
        std::shared_ptr<SpriteBatch> spriteBatch_;

        float depthValue = 1.0f;

        int viewWidth_;
        int viewHeight_;

        int createFrameBufferForVirtualTexture(std::shared_ptr<TextureInfo> texInfo);
        
        const std::string DEFAULT_TEXT_RENDER_NAME = "default_text_render";

        std::map<std::string , std::shared_ptr<TextRender>> textRenderMap_;
    };

    class TextRenderHelper{
    public:
        void loadRes(RenderEngine &engine);

        Shader textRenderShader_;

        std::shared_ptr<CharInfo> findCharInfo(wchar_t &ch);

        void layoutText(std::wstring &content , 
            TextRenderCommand *renderCmd,
            Rect &outRect,
            std::vector<float> &buf);

        unsigned int mainTextureId_;

        static void loadSymbolMap();

        static bool isSymbol(std::wstring &ch);

    private:
        void buildTextCharConfig();

        void addSpecialTextCharInfo();

        std::unordered_map<wchar_t , std::shared_ptr<CharInfo>> charInfoMaps_;
    };

    //渲染命令缓存
    class RenderCommandCache{
    public:
        std::shared_ptr<TextRenderCommand> acquireTextRender(std::wstring &content , Rect &rect ,TextPaint &paint);
    };
}


