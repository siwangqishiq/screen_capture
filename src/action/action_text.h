#pragma once

#include "action/menu.h"

class EditTextMenuItem : public MenuItem{
public:
    static std::string Name;

    EditTextMenuItem(Application *_app) : MenuItem(_app , Name,"ic_ed_text.png"){}
    
    virtual void onItemClick() override;
};