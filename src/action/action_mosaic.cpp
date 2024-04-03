
#include "action/action_mosaic.h"
#include "editor/editor_mosaic.h"

std::string MosaicMenuItem::Name = "mosaic";

void MosaicMenuItem::onItemClick(){
    purple::Log::w("MosaicMenuItem" , "MosaicMenuItem Clicked");
    //todo
    isSelected = !isSelected;
    if(isSelected){ //编辑按钮被选中
        clearOtherMenuItemSelectState();
        mApp->setCurrentEditor(std::make_shared<MosaicEditor>(mApp));
        // mApp->updateCursor(CursorType::Cross);
    }else{ //取消选中
        mApp->setCurrentEditor(nullptr);
        mApp->mState = ScreenState::CAPTURE_ZONE_GETTED;
        // mApp->updateCursor(CursorType::Normal);
    }
}