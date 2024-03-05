#include "menu.h"
#include "application.h"

#include "action_confirm.h"
#include "action_cancel.h"
#include "action_edit.h"

ActionMenu::ActionMenu(Application *_app){
    mApp = _app;
}

void ActionMenu::init(){
    addMenuItems();
}

void ActionMenu::addMenuItems(){
    
    //paint a rectangle
    std::shared_ptr<MenuItem> edRectItem = std::make_shared<EditPaintRectMenuItem>(this->mApp);
    mMenuItems.push_back(edRectItem);

    //paint a circle
    std::shared_ptr<MenuItem> edCircleItem = std::make_shared<EditPaintCircleMenuItem>(this->mApp);
    mMenuItems.push_back(edCircleItem);

    //paint pencil
    std::shared_ptr<MenuItem> edPencilItem = std::make_shared<EditPaintPencilMenuItem>(this->mApp);
    mMenuItems.push_back(edPencilItem);

    // confirm item
    std::shared_ptr<MenuItem> confirmItem = std::make_shared<ConfirmMenuItem>(this->mApp);
    mMenuItems.push_back(confirmItem);

    // cancel
    std::shared_ptr<MenuItem> cancelItem = std::make_shared<CancelMenuItem>(this->mApp);
    mMenuItems.push_back(cancelItem);
}

void ActionMenu::update(){
    if(mApp->mState != ScreenState::CAPTURE_ZONE_GETTED
        && mApp->mState != ScreenState::CAPTURE_ZONE_EDIT){
        return;
    }

    // CAPTURE_ZONE_GETTED
    resetMenuItemsPosition();

    float x = mApp->mEventX;
    float y = mApp->mScreenHeight - mApp->mEventY;
    for(auto item: mMenuItems){
        item->isMouseHove = false;
        auto rect = item->genItemWrapRect();
        if(purple::isPointInRect(rect , x , y)){
            item->isMouseHove = true;
        }
    }//end for each
}

void ActionMenu::render(){
    if(mApp->mState != ScreenState::CAPTURE_ZONE_GETTED && 
        mApp->mState != ScreenState::CAPTURE_ZONE_EDIT ){
        return;
    }

    //do render
    if(mApp->isReadPixelMode){ // no need to render
       return; 
    }
    
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

    if(top - findMenuHeight() <= offsetPadding){
        top = findMenuHeight() + offsetPadding;
    }
}

float ActionMenu::findMenuWidth(){
    float totalWidth = 0.0f;
    for(int i = mMenuItems.size() - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        totalWidth += item->mWidth;
    }//end for i
    return totalWidth;
}

float ActionMenu::findMenuHeight(){
    float totalWidth = 0.0f;
    for(int i = mMenuItems.size() - 1;i >= 0; i--){
        auto item = mMenuItems[i];
        if(item->mHeight > 0){
            return item->mHeight;
        }
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
    if(mApp->mState != ScreenState::CAPTURE_ZONE_GETTED 
        && mApp->mState != ScreenState::CAPTURE_ZONE_EDIT){
        return false;
    }

    bool ret = false;
    switch(action){
        case ActionDown:{
            int findIndex = isHitMenuItems(x , y);
            if(findIndex >= 0){
                mGrapMenuItem = mMenuItems[findIndex];
                mGrapMenuItem->isPressed = true;
                ret = true;
                // purple::Log::e("menu" , "grab menu wait...");
            }
            break;
        }
        case ActionMove:{
            if(mGrapMenuItem != nullptr){
                ret = true;
            }
            break;
        }
        case ActionUp:{
             if(mGrapMenuItem != nullptr){
                auto rect = mGrapMenuItem->genItemWrapRect();
                if(purple::isPointInRect(rect , x , y)){
                    mGrapMenuItem->onItemClick();
                    mGrapMenuItem->clearFlags();
                }
                ret = true;
                // purple::Log::e("menu" , "grab menu up will click?");
                mGrapMenuItem = nullptr;
            }
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
    purple::Rect dstRect;
    dstRect.left = mLeft;
    dstRect.top = mTop;
    dstRect.width = mWidth;
    dstRect.height = mHeight;

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    purple::Paint bgPaint;
    bgPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    shapeBatch->renderRect(dstRect , bgPaint);
    if(this->isMouseHove || isSelected){
        purple::Paint mskPaint;
        mskPaint.color = glm::vec4(0.0f , 0.0f , 0.0f ,0.2f);
        shapeBatch->renderRect(dstRect , mskPaint);
    }
    shapeBatch->end();


    auto spriteBatch = purple::Engine::getRenderEngine()->getSpriteBatch();
    spriteBatch->begin();

    purple::Rect srcRect = mIconImage->getRect();

    spriteBatch->renderImage(*mIconImage , srcRect , dstRect);
    spriteBatch->end();
}

void MenuItem::onItemClick(){
    purple::Log::w("menu" , "%s clicked" , this->mName.c_str());
}

