#include "application.h"
#include "action/action_confirm.h"
#include "utils.h"

std::string ConfirmMenuItem::Name = "confirm";

//确认
void ConfirmMenuItem::onItemClick(){
    // save image
    mApp->isReadPixelMode = true;
    mApp->render();

    doSaveImage();
   
    mApp->isReadPixelMode = false;
    mApp->mState = ScreenState::Idle; 
}

void ConfirmMenuItem::doSaveImage() const{
    std::vector<float> coords = mApp->calClipPoints();
    float left = coords[0];
    float right = coords[1];
    float top = coords[2];
    float bottom = coords[3];
    
    long t1 = purple::currentTimeMillis();
    int errCode = purple::Engine::exportImageFile(mApp->mOutputFilePath
        , static_cast<int>(left) 
        , static_cast<int>(bottom)
        , static_cast<int>(right - left) 
        , static_cast<int>(top - bottom));
    long t2 = purple::currentTimeMillis();
    if(errCode == 0){
        purple::Log::w("menu" , "save image cost time %ld ms" , t2 - t1);
        mApp->exit();
    }else{
        purple::Log::e("menu" , "save image error(%d)" , errCode);
    }
}
