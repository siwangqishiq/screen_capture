
#include "action/action_text.h"
#include "editor/editor_text.h"

std::string EditTextMenuItem::Name = "text";

void EditTextMenuItem::onItemClick(){
    purple::Log::i("menu" , "EditTextMenuItem Clicked");

    isSelected = !isSelected;
    if(isSelected){ //文本按钮被选中
        clearOtherMenuItemSelectState();
        
        if(mEditSetting != nullptr){
            mEditSetting->isVisible = true;
            mApp->setCurrentEditor(std::make_shared<TextEditor>(mApp,mEditSetting->getSelectedColor()));
        }
        mApp->updateCursor(CursorType::Normal);
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        if(mEditSetting != nullptr){
            mEditSetting->isVisible = false;
        }

        
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;
    }//end if
}
