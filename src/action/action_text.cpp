
#include "action/action_text.h"

std::string EditTextMenuItem::Name = "text";

void EditTextMenuItem::onItemClick(){
    purple::Log::i("menu" , "EditTextMenuItem Clicked");

    isSelected = !isSelected;
    if(isSelected){ //文本按钮被选中
        clearOtherMenuItemSelectState();

        if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = true;
            // mApp->setCurrentEditor(std::make_shared<CircleEditor>(mApp
            //     ,mEditSetting->getSelectedColor()
            //     ,mEditSetting->getSelectedSize()));
        }
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        // mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;

         if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = false;
        }
    }
}
