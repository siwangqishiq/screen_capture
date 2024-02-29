#include "action_menu.h"
#include "application.h"

ActionMenu::ActionMenu(Application *_app){
    mApp = _app;
}

void ActionMenu::init(){
    addMenuItems();
}

void ActionMenu::addMenuItems(){
    // confirm item
    std::shared_ptr<MenuItem> confirmItem = std::make_shared<MenuItem>(this, "confirm" , "ic_confirm.png");
    mMenuItems.push_back(confirmItem);

    std::shared_ptr<MenuItem> cancelItem = std::make_shared<MenuItem>(this, "cancel" , "ic_cancel.png");
    mMenuItems.push_back(cancelItem);
}

void ActionMenu::tick(){
    if(mApp->mState != ScreenState::CAPTURE_ZONE_GETTED){
        return;
    }

    float right = mApp->mScreenWidth;
    float top = mApp->mScreenHeight;
    locateMenuLocation(right , top);

    //render all items
    const int size = mMenuItems.size();
    for(int i = size - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        item->render(right - item->mWidth , top);
        right -= item->mWidth;
    }//end for i
}

//定位菜单位置
void ActionMenu::locateMenuLocation(float &right , float &top){

}

bool ActionMenu::onEventAction(EventAction action, float x , float y){
    return false;
}

void ActionMenu::dispose(){

}

void MenuItem::render(float left , float top){
    auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
    spriteBatch->begin();

    purple::Rect srcRect = mIconImage->getRect();
    purple::Rect dstRect;
    dstRect.left = left;
    dstRect.top = top;
    dstRect.width = mWidth;
    dstRect.height = mHeight;
    spriteBatch->renderImage(*mIconImage , srcRect , dstRect);
    spriteBatch->end();
}

void MenuItem::onItemClick(){

}

