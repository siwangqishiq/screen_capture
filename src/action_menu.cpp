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

    resetMenuItemsPosition();

    //render all items
    for(int i = mMenuItems.size() - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        item->render(item->mLeft , item->mTop);
    }//end for i
}

//定位菜单位置
void ActionMenu::locateMenuLocation(float &right , float &top){
    std::vector<float> result = mApp->calClipPoints();
    const float clipRight = result[1];
    const float clipBottom = result[3];

    const float offsetPadding = 8.0f; 
    right = clipRight;
    top = clipBottom - offsetPadding;
}

float ActionMenu::findMenuWidth(){
    float totalWidth = 0.0f;
    for(int i = mMenuItems.size() - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        totalWidth += item->mWidth;
    }//end for i
    return totalWidth;
}

void ActionMenu::resetMenuItemsPosition(){
    float right = mApp->mScreenWidth;
    float top = mApp->mScreenHeight;
    locateMenuLocation(right , top);

    for(int i = mMenuItems.size() - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        item->resetPostion(right - item->mWidth , top);
        right -= item->mWidth;
    }//end for i
}

 int ActionMenu::isHitMenuItems(float x , float y){
    for(std::vector<std::shared_ptr<MenuItem> >::size_type i = 0 ; i < mMenuItems.size() ;i++){
        purple::Rect rect = mMenuItems[i]->genItemWrapRect();
        if(purple::isPointInRect(rect , x , y)){
            return i;
        }
    }//end for i
    return -1;
 }

bool ActionMenu::dispatchEventAction(EventAction action, float x , float y){
    if(mApp->mState != ScreenState::CAPTURE_ZONE_GETTED){
        return false;
    }

    bool ret = false;
    switch(action){
        case ActionDown:{
            int findIndex = isHitMenuItems(x , y);
            if(findIndex >= 0){
                mGrapMenuItem = mMenuItems[findIndex];
                ret = true;
            }
            break;
        }
        case ActionUp:{
             if(mGrapMenuItem != nullptr){
                auto rect = mGrapMenuItem->genItemWrapRect();
                if(purple::isPointInRect(rect , x , y)){
                    mGrapMenuItem->onItemClick();
                }
            }
            mGrapMenuItem = nullptr;
            break;
        }
        default:
            break;
    }//end switch

    return ret;
}

void ActionMenu::dispose(){
}

void MenuItem::render(float left , float top){
    auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
    spriteBatch->begin();

    purple::Rect srcRect = mIconImage->getRect();
    purple::Rect dstRect;
    dstRect.left = mLeft;
    dstRect.top = mTop;
    dstRect.width = mWidth;
    dstRect.height = mHeight;
    spriteBatch->renderImage(*mIconImage , srcRect , dstRect);
    spriteBatch->end();
}

void MenuItem::onItemClick(){
    purple::Log::e("menu" , "%s menuitem clicked" , this->mName.c_str());
}

