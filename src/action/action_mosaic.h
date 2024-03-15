#pragma once

#include "action/menu.h"
#include <string>

class MosaicMenuItem : public MenuItem{
public:
    static std::string Name;

    MosaicMenuItem(Application *_app) : MenuItem(_app , Name,"ic_ed_mosaic.png"){}
    
    virtual void onItemClick() override;
};