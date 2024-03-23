#include "action/action_cancel.h"

// #include <shlobj.h>
#include "purple.h"
// #include <tchar.h>

std::string CancelMenuItem::Name = "cancel";

void CancelMenuItem::onItemClick(){
    doExit();
}

void CancelMenuItem::doExit() const{
    mApp->exit();
}