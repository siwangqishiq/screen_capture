#include "purple.h"
#include "ui/ui_root.h"
#include "ui/widget.h"
#include "ui/container.h"

namespace purple{
    UiRoot::UiRoot(int w, int h) {
        this->rootWidth_ = w;
        this->rootHeight_ = h;
        Log::i("ui", "UiRoot create width:%d, height:%d"
            ,this->rootWidth_
            ,this->rootHeight_);
    }

    UiRoot::~UiRoot(){
        Log::i("ui", "UiRoot destoryed");
    }

    void UiRoot::setRootContainer(PContainer container){
        this->rootContainer_ = container;
        container->setRootUi(this);
    }

    bool UiRoot::dispatchInputEvent(InputEvent &event){
        // Log::i("ui" , "event %d , %f %f" , event.action , event.x , event.y);
        if(rootContainer_ == nullptr){
            return false;
        }
        
        bool costEvent = false;
        if(targetWidget != nullptr){
            // std::cout << "targetWidget is not null id = " << targetWidget->id << std::endl;
            costEvent = targetWidget->onInputEvent(event);
        }else{
            // std::cout << "targetWidget is null" << std::endl;
            costEvent = rootContainer_->dispatchInputEvent(event);
        }

        if(event.action == EVENT_ACTION_END){
            targetWidget = nullptr;
        }
        return costEvent;
    }

    void UiRoot::setTargetWidget(Widget *target){
        this->targetWidget = target;
    }

    void UiRoot::measure(){
        // Log::i("widget" , "UiRoot start measure");
        if(this->rootContainer_ == nullptr){
            return;
        }

        int reqW = this->rootContainer_->getRequestWidth();
        MeasureSpecMode wMode = MeasureSpecMode::Unset;
        int widthValue = 0;
        if(reqW == LAYOUT_MATCH_PARENT){
            wMode = MeasureSpecMode::Exactly;
            widthValue = this->rootWidth_;
        }else if(reqW == LAYOUT_WRAP_CONTENT){
            wMode = MeasureSpecMode::Atmost;
            widthValue = this->rootWidth_;
        }else {
            wMode = MeasureSpecMode::Exactly;
            widthValue = reqW;
        }

        int reqH = this->rootContainer_->getRequestHeight();
        MeasureSpecMode hMode = MeasureSpecMode::Unset;
        int heightValue = 0;
        if(reqH == LAYOUT_MATCH_PARENT){
            hMode = MeasureSpecMode::Exactly;
            heightValue = this->rootHeight_;
        }else if(reqH == LAYOUT_WRAP_CONTENT){
            hMode = MeasureSpecMode::Atmost;
            heightValue = this->rootHeight_;
        }else {
            hMode = MeasureSpecMode::Exactly;
            heightValue = reqH;
        }

        this->rootContainer_->measure(wMode, widthValue, hMode, heightValue);
    }
    
    void UiRoot::layout(){
        // Log::i("widget" , "UiRoot start layout");
        if(this->rootContainer_ == nullptr){
            return;
        }

        this->rootContainer_->layout(0 , this->rootHeight_);
    }
    
    void UiRoot::render(){
        // Log::i("widget" , "UiRoot start render");
        if(this->rootContainer_ == nullptr){
            return;
        }

        this->rootContainer_->render();
    }

    void UiRoot::startRenderUI(){
        measure();
        layout();
        if(!purple::UNITTEST){
            render();
        }
    }
}
