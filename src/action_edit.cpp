#include "application.h"
#include "action_edit.h"

#include "editor_rect.h"


// --------------------RECT------------------------
std::string EditPaintRectMenuItem::Name = "paint_rect";
void EditPaintRectMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());

    isSelected = !isSelected;
    
    if(isSelected){ //编辑按钮被选中
        mApp->setCurrentEditor(std::make_shared<RectEditor>(mApp));
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;
    }
}


// --------------------CIRCLE------------------------
std::string EditPaintCircleMenuItem::Name = "paint_circle";
void EditPaintCircleMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());
}


// --------------------PENCIL------------------------
std::string EditPaintPencilMenuItem::Name = "paint_pencil";
void EditPaintPencilMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());
}