#include "application.h"
#include "action/action_edit.h"

#include "editor/editor_rect.h"
#include "editor/editor_circle.h"
#include "editor/editor_pencil.h"


// --------------------RECT------------------------
std::string EditPaintRectMenuItem::Name = "paint_rect";
void EditPaintRectMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());

    isSelected = !isSelected;
    
    if(isSelected){ //编辑按钮被选中
        clearOtherMenuItemSelectState();

        if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = true;
            mApp->setCurrentEditor(std::make_shared<RectEditor>(mApp,
                mEditSetting->getSelectedColor(),
                mEditSetting->getSelectedSize()));
        }
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;

        if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = false;
        }
    }
}


// --------------------CIRCLE------------------------
std::string EditPaintCircleMenuItem::Name = "paint_circle";
void EditPaintCircleMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());
    
    isSelected = !isSelected;
    if(isSelected){ //编辑按钮被选中
        clearOtherMenuItemSelectState();

        if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = true;
            mApp->setCurrentEditor(std::make_shared<CircleEditor>(mApp
                ,mEditSetting->getSelectedColor()
                ,mEditSetting->getSelectedSize()));
        }
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;

         if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = false;
        }
    }
}


// --------------------PENCIL------------------------
std::string EditPaintPencilMenuItem::Name = "paint_pencil";
void EditPaintPencilMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());

    isSelected = !isSelected;
    if(isSelected){ //编辑按钮被选中
        clearOtherMenuItemSelectState();

        if(this->mEditSetting != nullptr){
            mEditSetting->isVisible = true;
            mApp->setCurrentEditor(std::make_shared<PencilEditor>(mApp 
                ,mEditSetting->getSelectedColor()
                ,mEditSetting->getSelectedSize()));
        }
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;

        if(this->mEditSetting != nullptr){
            this->mEditSetting->isVisible = false;
        }
    }
}