#pragma once

#include "editor/editor.h"
#include "purple.h"

class Application;

enum TextEditorState{
    WaitInputPosition,//等待确定文本输入位置
    InputMode,//输入模式
    Adjust//调整模式
};

class TextEditor : public BaseEditor{
public:
    TextEditor(Application *_app , glm::vec4 color) : BaseEditor(_app){
        mTextColor = color;
        mPaint.fillStyle = purple::FillStyle::Stroken;
        mPaint.color = glm::vec4(0.0f , 0.0f ,0.0f , 1.0f);
        mPaint.stokenWidth = 2.0f;

        // std::vector<float> results = mApp->calClipPoints();
        // float cx = (results[0] +  results[1]) / 2.0f;
        // float cy = (results[2] +  results[3]) / 2.0f;
    }

    virtual void setStrokenWidth(float width) override;

    virtual void setColor(glm::vec4 color) override;
    
    virtual void renderEditorContent() override;
    
    virtual void endPaint();

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;

    virtual void onInputContentChange(std::wstring content) override;
private:
    TextEditorState mTextEditorState = TextEditorState::WaitInputPosition;
    bool mWaitingUpAction = false;

    glm::vec4 mTextColor = glm::vec4(0.0f , 0.0f ,0.0f ,1.0f);
    purple::Rect mDstRect;
    float mPadding = 8.0f;
    float mTextBoxHeight = 32.0f;
    float mTextBoxWidth = 32.0f;

    float mTextBoxLeft = 0.0f;
    float mTextBoxTop = 0.0f;

    std::wstring mInputContent = L"";

    std::shared_ptr<purple::TextureInfo> mCharsTex = nullptr;

    purple::Rect mTextWrapRect;

    void rebuildCharsTexture();

    void doRenderCharsTex(int texWidth , int texHeight);

    void confirmInputTextPosition(float x , float y);
};