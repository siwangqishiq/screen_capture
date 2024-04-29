#pragma once

#include "render/cmd/command.h"

namespace purple{
    // 文本渲染命令
    class TextRenderCommand : public RenderCommand{
    public:
        const int vertCountPerChar_ = 6;//一个字符由几个顶点确定

        TextRenderCommand(RenderEngine *engine) : RenderCommand(engine) {
        }

        void putParams(std::wstring text , float left , float bottom 
                        ,TextPaint &paint);

        virtual void putTextParamsByRectLimit(std::wstring &text , Rect &limitRect, 
                Rect *wrapContentRect,
                TextPaint &paint);

        virtual void runCommands();
        
        void setPaint(TextPaint &paint){
            paint_ = paint;
        }

        void putVertexDataToBuf(std::vector<float> &buf, int index, 
                                float x ,float y,float depth,
                                std::shared_ptr<CharInfo> charInfo ,
                                TextPaint &paint);
        
        void updateVertexPositionData(std::vector<float> &buf, int index, 
                                float x ,float y);

        virtual void buildGlCommands(std::vector<float> &buf);

        virtual Rect createEmptyWrapRect(Rect &limitRect , TextPaint &paint);
        
        TextPaint paint_;
        Rect limitRect_;

        //每个顶点包含的属性长度 
        //位置 postion 3 + 纹理坐标 uvw 3
        int attrCount_ = 3 + 3; 
    protected:
        unsigned int allocatorVRamForText(int textLength);
        unsigned int fontTextureId_ = -1;
    private:
        glm::vec4 textColor_;

        void fillTextVertData(std::wstring &text , float left , float bottom , 
                TextPaint &paint);
        
        float findCharMaxHeight(std::wstring &text , TextPaint &paint);
        
        float calOffsetY(std::shared_ptr<CharInfo> charInfo , float scale = 1.0f);

        float calTextStyleItalicOffset(std::shared_ptr<CharInfo> charInfo , TextPaint &paint);

    };
}