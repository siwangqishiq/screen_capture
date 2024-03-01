#pragma once

#include "menu.h"
#include <string>

class CancelMenuItem : public MenuItem{
public:
    static std::string Name;

    CancelMenuItem(Application *_app) : MenuItem(_app , Name,"ic_cancel.png"){}
    
    virtual void onItemClick() override;

    void doExit() const;
};