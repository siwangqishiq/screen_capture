
#include "editor/editor_text.h"
#include "config/constants.h"

void TextEditor::renderEditorContent() {
    // purple::Log::e("rect" , "renderEditorContent mVisible = %d mTextEditorState = %d" 
    //     , mVisible , mTextEditorState);
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

        
    const float offset = 12.0f;
    mControlRect.left = mDstRect.left - offset;
    mControlRect.top = mDstRect.top + offset;
    mControlRect.width = mDstRect.width + 2.0f * offset;
    mControlRect.height = mDstRect.height + 2.0f * offset;
   
    if(mApp->isReadPixelMode || !isShowControlButton){
       return; 
    }

    //render control box

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(mControlRect , mPaint);
    shapeBatch->end();

    //btn del and scale  rotate????
    
    auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
    spriteBatch->begin();
    auto delImageRect = mDelImage->getRect();
  
    mDelDstRect.width = mBtnSize;
    mDelDstRect.height = mBtnSize;
    mDelDstRect.left = mControlRect.getRight() - mBtnSize / 2.0f;
    mDelDstRect.top = mControlRect.getTop() + mBtnSize / 2.0f;

    auto scaleImageRect = mScaleImage->getRect();
    purple::Rect scaleDstRect;
    mScaleDstRect.width = mBtnSize;
    mScaleDstRect.height = mBtnSize;
    mScaleDstRect.left = mControlRect.getRight() - mBtnSize / 2.0f;
    mScaleDstRect.top = mControlRect.getBottom() + mBtnSize / 2.0f;

    spriteBatch->renderImage(mDelImage , delImageRect , mDelDstRect);
    spriteBatch->renderImage(mScaleImage , scaleImageRect , mScaleDstRect);

    spriteBatch->end();
}

void TextEditor::setStrokenWidth(float width){
    //do nothing
}

void TextEditor::setColor(glm::vec4 color) {
    mTextColor = color;
    rebuildCharsTexture();
}

int TextEditor::editorType() {
    return EditorType::DrawText;
}
    
void TextEditor::endPaint() {
    
}

bool TextEditor::onTouchEvent(EventAction action , float x , float y){
    return dispatchEventAction(action , x ,y);
}

bool TextEditor::dispatchEventAction(EventAction action , float x , float y){
    std::vector<float> results = mApp->calClipPoints();
    float left = results[0];
    float right = results[1];
    float top = results[2];
    float bottom = results[3];

    purple::Rect captureContentRect(left , top , right - left , top - bottom);
    
    if(action == EventAction::ActionDown){
        if(mTextEditorState == WaitInputPosition){
            if(purple::isPointInRect(captureContentRect , x , y)){
                mStartX = x;
                mStartY = y;
                mEndX = x;
                mEndY = y;

                mVisible = true;
                mWaitingUpAction = true;

                return true;
            }
        }else if(mTextEditorState == InputMode){
            if(purple::isPointInRect(mDelDstRect , x , y) && isShowControlButton){
                mWaitDelButtonClicked = true;
                return true;
            }
        }
    }else if(action == EventAction::ActionMove){
        return false;
    }else if(action == EventAction::ActionUp){
        bool ret = false;
        if(!mVisible){
            return ret;
        }

        if(mWaitDelButtonClicked && purple::isPointInRect(mDelDstRect , x , y)){ //点击删除按钮
            onClickDelButton();
            ret = true;
        }else if(mWaitingUpAction){ //确定文本插入位置
            if(mTextEditorState == WaitInputPosition){
                mTextEditorState = InputMode;
            }
            confirmInputTextPosition(x , y);
            ret = true;
        }//end if


        mWaitingUpAction = false;
        mWaitDelButtonClicked = false;
        
        return ret;
    }//end if
    return false;
}

void TextEditor::confirmInputTextPosition(float x , float y){
    mApp->mInputContent = L"";
    mInputContent = L"";
    
    mTextBoxLeft = x;
    mTextBoxTop = y;

    isShowControlButton = true;
}

void TextEditor::onClickDelButton(){
    // purple::Log::e("TextEditor" , "onClickDelButton!!!!");
    if(mApp->mCurrentEditor!= nullptr && mApp->mCurrentEditor.get() == this){
        purple::Log::e("TextEditor" , "onClickDelButton this eidtor current not added in list");
        
        mTextEditorState = TextEditorState::WaitInputPosition; 
        mVisible = false;
        isShowControlButton = false;

        mApp->mInputContent = L"";
        onInputContentChange(L"");
    }else{
        //search in editor list
    }
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
        "chars_texs" , 2 * 1024, purple::FONT_DEFAULT_SIZE,
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

    purple::Log::e("textEidtor" , "mInputContent size = %d  mTextWrapRect (%f , %f , %f ,%f)" 
        , mInputContent.size()
        , mTextWrapRect.left
        , mTextWrapRect.top
        , mTextWrapRect.width
        , mTextWrapRect.height);

    // purple::Log::e("textEidtor" , "rebuildCharsTexture color (%f , %f , %f)" , 
    //     mTextColor[0],mTextColor[1],mTextColor[2]);
}