
#include "editor/editor_text.h"

void TextEditor::renderEditorContent() {
    if(!mVisible || mTextEditorState == WaitInputPosition){
        return;
    }

    //输入模式 or 调整模式
    float ratio = mTextWrapRect.width / mTextWrapRect.height;
    mTextBoxWidth = ratio * mTextBoxHeight;

    mDstRect.left = mTextBoxLeft;
    mDstRect.top = mTextBoxTop;
    mDstRect.height = mTextBoxHeight;
    mDstRect.width = mTextBoxWidth;

    if(mCharsTex != nullptr){ //render text content
        auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
        spriteBatch->begin();
        purple::TextureImage image(mCharsTex);
        spriteBatch->renderImage(image , mTextWrapRect , mDstRect);
        spriteBatch->end();
    }

   
    if(mApp->isReadPixelMode){
       return; 
    }

    //render control box
    purple::Rect controlRect;
    float offset = 8.0f;
    controlRect.left = mDstRect.left - offset;
    controlRect.top = mDstRect.top + offset;
    controlRect.width = mDstRect.width + 2.0f * offset;
    controlRect.height = mDstRect.height + 2.0f * offset;

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(controlRect , mPaint);
    shapeBatch->end();
}

void TextEditor::setStrokenWidth(float width){
    //do nothing
}

void TextEditor::setColor(glm::vec4 color) {
    mTextColor = color;
    rebuildCharsTexture();
}
    
void TextEditor::endPaint() {
    
}

bool TextEditor::dispatchEventAction(EventAction action , float x , float y){
    std::vector<float> results = mApp->calClipPoints();
    float left = results[0];
    float right = results[1];
    float top = results[2];
    float bottom = results[3];

    purple::Rect captureContentRect(left , top , right - left , top - bottom);

    if(action == EventAction::ActionDown){
        if(purple::isPointInRect(captureContentRect , x , y)){
            mStartX = x;
            mStartY = y;
            mEndX = x;
            mEndY = y;

            mVisible = true;
            mWaitingUpAction = true;


            return true;
        }
    }else{
        // purple::Log::e("eidtor" , "action move or up  action = %d" , action);
        if(!mVisible){
            return false;
        }

        // limitInRect(captureContentRect , x , y);

        if(action == EventAction::ActionMove){
        }else if(action == EventAction::ActionUp){
          if(mWaitingUpAction){
            mWaitingUpAction = false;
            if(mTextEditorState == WaitInputPosition){
                mTextEditorState = InputMode;
            }
            confirmInputTextPosition(x , y);
          }
        }//end if
        return true;
    }
    return false;
}

void TextEditor::confirmInputTextPosition(float x , float y){
    mApp->mInputContent = L"";
    mInputContent = L"";
    
    mTextBoxLeft = x;
    mTextBoxTop = y;
}

//输入文本改变 需要重新生成动态纹理
void TextEditor::onInputContentChange(std::wstring content){
    if(mInputContent != content){
        mInputContent = content;
        rebuildCharsTexture();
    }
}

void TextEditor::rebuildCharsTexture(){
    // purple::Log::e("TextEditor" , "rebuildCharsTexture mInputContent size = %d" , mInputContent.size());
    if(mCharsTex == nullptr){
        mCharsTex = purple::Engine::getRenderEngine()->buildVirtualTexture(
        "chars_texs" , 1024, purple::FONT_DEFAULT_SIZE,
        [this](int w, int h){
            doRenderCharsTex(w , h);
        });
    }else{
        purple::Engine::getRenderEngine()->updateVirtualTexture(mCharsTex, 
        [this](int w, int h){
            doRenderCharsTex(w , h);
        });
    }
}

void TextEditor::doRenderCharsTex(int texWidth , int texHeight){
    purple::Rect inputRect(0.0f , static_cast<float>(texHeight),
        static_cast<float>(texWidth) , static_cast<float>(texHeight));
    purple::TextPaint paint;
    paint.setTextSize(texHeight);
    paint.textColor = mTextColor;

    purple::Engine::getRenderEngine()->renderTextWithRect(mInputContent,
        inputRect ,paint , &mTextWrapRect);

    // purple::Log::e("textEidtor" , "rebuildCharsTexture color (%f , %f , %f)" , 
    //     mTextColor[0],mTextColor[1],mTextColor[2]);
}