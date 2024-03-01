#include "action_cancel.h"

std::string CancelMenuItem::Name = "cancel";

void CancelMenuItem::onItemClick(){
    doExit();
}

void CancelMenuItem::doExit() const{
    mApp->exit();
}