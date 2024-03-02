#include "application.h"
#include "action_edit.h"


// --------------------RECT------------------------
std::string EditPaintRectMenuItem::Name = "paint_rect";
void EditPaintRectMenuItem::onItemClick(){
    purple::Log::i("menu" , "%s menu click." , this->mName.c_str());
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