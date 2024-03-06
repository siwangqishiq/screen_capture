#include "application.h"
#include "action_edit.h"

#include "editor_rect.h"
#include "editor_circle.h"
#include "editor_pencil.h"


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

    isSelected = !isSelected;
        if(isSelected){ //编辑按钮被选中
        mApp->setCurrentEditor(std::make_shared<CircleEditor>(mApp));
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;
    }
}


// --------------------PENCIL------------------------
std::string EditPaintPencilMenuItem::Name = "paint_pencil";
void EditPaintPencilMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());

    isSelected = !isSelected;
        if(isSelected){ //编辑按钮被选中
        mApp->setCurrentEditor(std::make_shared<PencilEditor>(mApp));
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;
    }
}