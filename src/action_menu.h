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

// 菜单项
class MenuItem: public IMenuItemCallback{
public:
    MenuItem(ActionMenu *menu , std::string _name , std::string _icon) 
        : mMenu(menu) , mName(_name) , mIcon(_icon){
        mIconImage = purple::BuildImageByAsset(mIcon);
    }
    
    void render(float left , float top);

    virtual void onItemClick() override;

    ~MenuItem(){}

    float mWidth = 64.0f;
    float mHeight = 64.0f;
private:
    ActionMenu *mMenu;
    std::string mName;
    std::string mIcon;

    std::shared_ptr<purple::TextureImage> mIconImage;
};

//操作菜单
class ActionMenu{
public:
    ActionMenu(Application *_app);

    void init();
    void tick();
    void dispose();
    
    bool onEventAction(EventAction action , float x , float y);

    virtual ~ActionMenu(){
        purple::Log::i("ActionMenu" , "ActionMenu deconstructor.");
    }

    void locateMenuLocation(float &right , float &top);

    void addMenuItems();
private:
    Application *mApp;
    glm::vec4 mBgColor = glm::vec4(0.75f , 0.75f , 0.75f ,1.0f);
    std::vector<std::shared_ptr<MenuItem>> mMenuItems;
};