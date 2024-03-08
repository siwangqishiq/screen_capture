#pragma once

#include "application.h"
#include <string>
#include <vector>
#include <memory>
#include "purple.h"

class IMenuItemCallback{
public:
    virtual void onItemClick() = 0;
};

class MenuItem;
class EditPaintSetting{
private:
    float mLeft;
    float mTop;
    float mItemWidth = 25.0f;
    float mHeight = 40.0f;
    float mWidth = 0.0f;

    float mMargin = 8.0f;

    std::vector<glm::vec4> mColors = 
        {
            glm::vec4(1.0f , 0.0f , 0.0f , 1.0f),
            glm::vec4(1.0f , 0.5f , 0.0f , 1.0f),
            glm::vec4(0.0f , 0.0f , 1.0f , 1.0f),
            glm::vec4(0.0f , 1.0f , 0.0f , 1.0f),
            glm::vec4(0.0f , 0.0f , 0.0f , 1.0f),
            glm::vec4(1.0f , 1.0f , 1.0f , 1.0f)
        };
    
    std::vector<float> mSizes = {3.0f , 6.0f , 12.0f};

    int mCurrentSizeIndex = 1;
    int mCurrentColorIndex = 0;
public:
    EditPaintSetting(MenuItem *_item) : mMenuItem(_item){}

    bool isVisible = false;
    
    void update();
    void render();

    bool dispatchEventAction(EventAction action , float x , float y);

    MenuItem *mMenuItem;
};

// 菜单项
class MenuItem: public IMenuItemCallback{
public:
    MenuItem(Application *_app , std::string _name , std::string _icon) 
        : mApp(_app) , mName(_name) , mIcon(_icon){
        mIconImage = purple::BuildImageByAsset(mIcon);
    }
    
    virtual void render(float left , float top);

    virtual void onItemClick() override;

    ~MenuItem(){}

    float mWidth = 50.0f;
    float mHeight = 50.0f;
    float mLeft = 0.0f;
    float mTop = 0.0f;

    bool isMouseHove = false;
    bool isPressed = false;

    purple::Rect genItemWrapRect(){
        return purple::Rect(mLeft , mTop , mWidth , mHeight);
    }

    void resetPostion(float l , float t){
        mLeft = l;
        mTop = t;
    }

    void clearOtherMenuItemSelectState();

    void clearFlags(){
        isMouseHove = false;
        isPressed = false;
    }

    Application *mApp;
    std::string mName;
    std::string mIcon;
    bool isSelected = false;
    
    std::shared_ptr<EditPaintSetting> mEditSetting = nullptr;

protected:
    std::shared_ptr<purple::TextureImage> mIconImage;
};

//操作菜单
class ActionMenu{
public:
    ActionMenu(Application *_app);

    void init();
    void update();
    void dispose();

    void render();
    
    bool dispatchEventAction(EventAction action , float x , float y);

    virtual ~ActionMenu(){
        purple::Log::i("ActionMenu" , "ActionMenu deconstructor.");
    }

    void locateMenuLocation(float &right , float &top);

    void addMenuItems();

    std::shared_ptr<MenuItem> findMenuItemByName(std::string name) const;

    std::shared_ptr<MenuItem> mGrapMenuItem = nullptr;

    std::vector<std::shared_ptr<MenuItem>> getMenuItems() const{
        return mMenuItems;
    }
private:
    Application *mApp;
    glm::vec4 mBgColor = glm::vec4(0.75f , 0.75f , 0.75f ,1.0f);
    std::vector<std::shared_ptr<MenuItem>> mMenuItems;


    float mItemGapSize = 8.0f;

    float findMenuWidth();

    float findMenuHeight();

    //重置menuitem位置
    void resetMenuItemsPosition();

    int isHitMenuItems(float x , float y);
};

