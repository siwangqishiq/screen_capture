#include "action/action_cancel.h"

// #include <shlobj.h>
#include "purple.h"
// #include <tchar.h>

std::string CancelMenuItem::Name = "cancel";

void CancelMenuItem::onItemClick(){
    // BROWSEINFO bi;
    // bi.hwndOwner = nullptr;
    // bi.pidlRoot = CSIDL_DESKTOP;
    // bi.pszDisplayName = nullptr;
    // bi.lpszTitle  = nullptr;
    // bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    // bi.lpfn = nullptr;
    // bi.iImage = 0;
    // auto pidl = SHBrowseForFolder(&bi);

    // TCHAR *strFolder = new TCHAR[1024];
    // SHGetPathFromIDList(pidl , strFolder);
    // std::string wStrFolder = std::string(&strFolder[0]);
    // purple::Log::e("file" , "select->%s" , wStrFolder.c_str());

    doExit();
}

void CancelMenuItem::doExit() const{
    mApp->exit();
}