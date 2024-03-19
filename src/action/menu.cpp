#include "action/menu.h"
#include "application.h"

#include "action/action_confirm.h"
#include "action/action_cancel.h"
#include "action/action_edit.h"
#include "action/action_mosaic.h"
#include "action/action_text.h"
#include "editor/editor.h"

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
    edRectItem->mEditSetting = std::make_shared<EditPaintSetting>(edRectItem.get());

    //paint a circle
    std::shared_ptr<MenuItem> edCircleItem = std::make_shared<EditPaintCircleMenuItem>(this->mApp);
    mMenuItems.push_back(edCircleItem);
    edCircleItem->mEditSetting = std::make_shared<EditPaintSetting>(edCircleItem.get());
    
    //paint pencil
    std::shared_ptr<MenuItem> edPencilItem = std::make_shared<EditPaintPencilMenuItem>(this->mApp);
    mMenuItems.push_back(edPencilItem);
    edPencilItem->mEditSetting = std::make_shared<EditPaintSetting>(edPencilItem.get());

    // mosaic
    std::shared_ptr<MenuItem> mosaicItem = std::make_shared<MosaicMenuItem>(this->mApp);
    mMenuItems.push_back(mosaicItem);

    //text add
    std::shared_ptr<MenuItem> textItem = std::make_shared<EditTextMenuItem>(this->mApp);
    mMenuItems.push_back(textItem);
    textItem->mEditSetting = std::make_shared<EditPaintSetting>(textItem.get());

    // confirm item
    std::shared_ptr<MenuItem> confirmItem = std::make_shared<ConfirmMenuItem>(this->mApp);
    mMenuItems.push_back(confirmItem);

    // cancel
    std::shared_ptr<MenuItem> cancelItem = std::make_shared<CancelMenuItem>(this->mApp);
    mMenuItems.push_back(cancelItem);
}

std::shared_ptr<MenuItem> ActionMenu::findMenuItemByName(std::string name) const{
    for(auto item : mMenuItems){
        if(item->mName == name){
            return item;
        }
    }//end for each
    return nullptr;
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
        
        if(item->mEditSetting != nullptr){
            item->mEditSetting->update();
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

 bool ActionMenu::isHitEditPanel(float x , float y){
    for(unsigned int i = 0 ; i < mMenuItems.size() ;i++){
        auto editPanel = mMenuItems[i]->mEditSetting;
        if(editPanel != nullptr && editPanel->isVisible){
            auto rect = editPanel->genWrapRect();
            if(purple::isPointInRect(rect , x , y)){
                mEditPaintSetting = editPanel;
                mEditPaintSetting->dispatchEventAction(EventAction::ActionDown , x, y);
                mEditPaintSetting->onActionDown(x , y);
                return true;
            }
        }//end if
    }//end for i
     return false;
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

            isHitEditPanel(x , y);
            break;
        }
        case ActionMove:{
            if(mGrapMenuItem != nullptr){
                ret = true;
            }
            if(mEditPaintSetting != nullptr){
                ret = true;
                mEditPaintSetting->dispatchEventAction(ActionMove , x , y);
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

            if(mEditPaintSetting != nullptr){
                mEditPaintSetting->dispatchEventAction(ActionUp , x , y);
                mEditPaintSetting->onActionUp(x , y);
                ret = true;
                mEditPaintSetting = nullptr;
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

void MenuItem::clearOtherMenuItemSelectState(){
    for(std::shared_ptr<MenuItem> menuItem : mApp->mActionMenu->getMenuItems()){
        if(menuItem.get() != this){
            menuItem->isSelected = false;

            if(menuItem->mEditSetting != nullptr){
                menuItem->mEditSetting->isVisible = false;
            }
        }
    }//end for each
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

    if(this->mEditSetting != nullptr){
        this->mEditSetting->render();
    }//end if
}

void MenuItem::onItemClick(){
    purple::Log::w("menu" , "%s clicked" , this->mName.c_str());
}

void EditPaintSetting::update(){
    //measure and location
    int itemCount = mSizes.size() + mColors.size();
    mWidth = itemCount * mItemWidth;

    purple::Rect menuRect = mMenuItem->genItemWrapRect();

    mTop = menuRect.getBottom() - mMargin;
    if(mTop - mHeight <= 0.0f){
        mTop = menuRect.getTop() + mMargin;
    }

    mLeft = menuRect.getLeft() + menuRect.width / 2.0f - mWidth / 2.0f ;
}

void EditPaintSetting::render(){
    if(!isVisible){
        return;
    }

    purple::Paint paint;
    paint.color = glm::vec4(1.0f , 1.0f , 1.0f, 1.0f);
    paint.fillStyle = purple::FillStyle::Filled;

    purple::Rect rect(mLeft , mTop , mWidth , mHeight);

    float centerX = rect.center().x;
    
    purple::Engine::getRenderEngine()->renderTriangle(
        centerX - mItemWidth / 5.0f , mTop,
        centerX + mItemWidth / 5.0f , mTop,
        centerX , mTop + mMargin /2.0f,
        paint
    );

    auto shapeBatch = purple::Engine::getRenderEngine()->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRoundRect(rect , 4.0f , paint);

    unsigned int index = 0;
    float x = rect.left;
    float y = rect.top;
    //render storken width panel
    purple::Paint grayPaint;
    grayPaint.color = glm::vec4(0.75f , 0.75f , 0.75f ,1.0f);

    purple::Paint selectedPaint;
    selectedPaint.color = getSelectedColor();
    if(mCurrentColorIndex == mColors.size() - 1){
        selectedPaint.fillStyle = purple::FillStyle::Stroken;
        selectedPaint.color = glm::vec4(0.0f , 0.0f , 0.0f ,1.0f);
    }
    for(index = 0; index < this->mSizes.size(); index++){
        if(mCurrentSizeIndex == index){
            shapeBatch->renderCircle(x + index * mItemWidth + mItemWidth / 2.0f 
                , y - mHeight / 2.0f , mSizes[index] , selectedPaint);
        }else{
            shapeBatch->renderCircle(x + index * mItemWidth + mItemWidth / 2.0f 
                , y - mHeight / 2.0f , mSizes[index] , grayPaint);
        }
        
    }//end for index

    //render color panel
    float colorCubeWidth = mItemWidth * 0.8f;
    float colorCubeHeight = colorCubeWidth;
    
    for(;index < mSizes.size() + mColors.size() ; index++){
        unsigned int colorIndex = index - mSizes.size();
        float left = x + index * mItemWidth + (mItemWidth - colorCubeWidth) / 2.0f;
        float top = y - (mHeight - colorCubeHeight) / 2.0f;
        purple::Rect colorCubeRect(left , top , colorCubeWidth , colorCubeHeight);
        purple::Paint colorCubePaint;
        colorCubePaint.color = mColors[colorIndex];

        if(mSizes.size() + mColors.size() - 1 == index){ // last is white show a stoken rect
            colorCubePaint.fillStyle = purple::FillStyle::Stroken;
            colorCubePaint.color = glm::vec4(0.75f , 0.75f , 0.75f ,1.0f);
        }

        shapeBatch->renderRect(colorCubeRect , colorCubePaint);
        if(colorIndex == mCurrentColorIndex){
            purple::Paint colorSelectPaint;
            colorSelectPaint.color = glm::vec4(1.0f , 1.0f ,1.0f ,1.0f);
            if(mSizes.size() + mColors.size() - 1 == index){ // last is white 
                colorSelectPaint.color = glm::vec4(0.75f , 0.75f , 0.75f ,1.0f);
            }
            auto selectRect = colorCubeRect;
            selectRect.width = 0.5f*colorCubeRect.width;
            selectRect.height = 0.5f*colorCubeRect.height;
            selectRect.left += selectRect.width / 2.0f;
            selectRect.top -= selectRect.height / 2.0f;

            shapeBatch->renderRect(selectRect , colorSelectPaint);
        }
    }//end for index

    shapeBatch->end();
}

bool EditPaintSetting::dispatchEventAction(EventAction action , float x , float y){
    return false;
}

int EditPaintSetting::findActionIndex(float x , float y){
    purple::Rect rect = genWrapRect();
    if(purple::isPointInRect(rect , x , y)){
        float offset = x - rect.left;
        int itemCount = mSizes.size() + mColors.size();
        const int retIndex = static_cast<int>(offset / mItemWidth);
        if(retIndex >= itemCount){
            return -1;
        }
        return retIndex;
    }
    return -1;
}

void EditPaintSetting::onActionDown(float x , float y){
    // purple::Log::e("EditPaintSetting" , "onActionDown %f ,%f" , x , y);
    mOnActionDownIndex = findActionIndex(x , y);
}

void EditPaintSetting::onActionUp(float x , float y){
    // purple::Log::e("EditPaintSetting" , "onActionUp %f ,%f" , x , y);
    int currentIndex = findActionIndex(x , y);
    if(mOnActionDownIndex >=0 && mOnActionDownIndex == currentIndex){
        onClickItem(currentIndex);
    }
    mOnActionDownIndex = -1;
}

bool EditPaintSetting::onClickItem(int clickIndex){
    purple::Log::w("EditPaintSetting" , "onClickItem %d" , clickIndex);

    int sizesLimit = static_cast<int>(mSizes.size());
    int colorLimit = static_cast<int>(mColors.size());
    if(clickIndex >= 0 && clickIndex < sizesLimit){
        mCurrentSizeIndex = clickIndex;
        if(mMenuItem->mApp->mCurrentEditor != nullptr){
            mMenuItem->mApp->mCurrentEditor->setStrokenWidth(getSelectedSize());
        }
        return true;
    }else if(sizesLimit <= clickIndex && clickIndex < sizesLimit + colorLimit){
        mCurrentColorIndex = clickIndex - sizesLimit;
        if(mMenuItem->mApp->mCurrentEditor != nullptr){
            mMenuItem->mApp->mCurrentEditor->setColor(getSelectedColor());
        }
        return true;
    }
    return false;
}

