#pragma once

#include "menu.h"
#include <string>


//绘制矩形
class EditPaintRectMenuItem : public MenuItem{
public:
    static std::string Name;

    EditPaintRectMenuItem(Application *_app) : MenuItem(_app , Name,"ic_ed_rect.png"){}
    
    virtual void onItemClick() override;
};

//绘制圆形
class EditPaintCircleMenuItem : public MenuItem{
public:
    static std::string Name;

    EditPaintCircleMenuItem(Application *_app) : MenuItem(_app , Name,"ic_ed_round.png"){}
    
    virtual void onItemClick() override;
};

//画笔绘制
class EditPaintPencilMenuItem : public MenuItem{
public:
    static std::string Name;

    EditPaintPencilMenuItem(Application *_app) : MenuItem(_app , Name,"ic_ed_pencil.png"){}
    
    virtual void onItemClick() override;
};


