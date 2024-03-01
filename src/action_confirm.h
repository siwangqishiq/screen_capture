#pragma once

#include "menu.h"
#include <string>

class ConfirmMenuItem : public MenuItem{
public:
    static std::string Name;

    ConfirmMenuItem(Application *_app) : MenuItem(_app , Name,"ic_confirm.png"){}
    
    virtual void onItemClick() override;

    void doSaveImage() const;
};