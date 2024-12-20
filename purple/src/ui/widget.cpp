#include "ui/widget.h"
#include "purple.h"
#include "ui/container.h"
#include "ui/ui_root.h"

namespace purple{
    void Widget::setParentWidget(Container *parent){
        this->parent_ = parent;
    }

    int Widget::contentWidth(){
        return 0;
    }

    int Widget::contentHeight(){
        return 0;
    }

    void Widget::onMeasure(MeasureSpecMode widthSpecMode, 
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue){
        // std::cout << "Widget::onMeasure" << std::endl;
        if(widthSpecMode == MeasureSpecMode::Exactly){
            setWidth(widthValue);
        }else if(widthSpecMode == MeasureSpecMode::Atmost){
            setWidth(std::min(paddingLeft_ + contentWidth() + paddingRight_, widthValue));
        }else if(widthSpecMode == MeasureSpecMode::Unset){
            setWidth(0);
        }

        if(heightSpecMode == MeasureSpecMode::Exactly){
            setHeight(heightValue);
        }else if(heightSpecMode == MeasureSpecMode::Atmost){
            setHeight(std::min(paddingTop_ + contentHeight() + paddingBottom_, heightValue));
        }else if(heightSpecMode == MeasureSpecMode::Unset){
            setHeight(0);
        }
    }

    void Widget::onLayout(int l,int t){
        this->left = l;
        this->top = t;
        // Log::w("test" , "widget id: %s pos: %d , %d" , this->id.c_str(), this->left , this->top);
    }

    void Widget::onRender(){
        // Log::e("render" , "render %d %d %d %d",this->left , this->top , this->width , this->height);
        if(this->bgColor_ == ConverColorValue(Color::Transparent)){
            return;
        }

        Rect bgRect(this->left , this->top , this->width_ , this->height_);
        Paint bgPaint;
        bgPaint.color = this->bgColor_;

        auto render = purple::Engine::getRenderEngine();
        auto shapeBatch = render->getShapeBatch();
        shapeBatch->begin();
        // shapeBatch->renderRect(bgRect, bgPaint);
        shapeBatch->renderRoundRect(bgRect, bgConnerRadius_ , bgPaint);
        shapeBatch->end();
    }

    Widget::~Widget(){
        // Log::i("widget","widget desstory");
    }

    void Widget::measure(MeasureSpecMode widthSpecMode, 
                        int widthValue, 
                        MeasureSpecMode heightSpecMode,
                        int heightValue) {
        if(visible_ == Gone){
            return;
        }

        onMeasure(widthSpecMode , widthValue, heightSpecMode , heightValue);
    }
    void Widget::layout(int l , int t){
        if(visible_ == Gone){
            return;
        }

        onLayout(l,t);
    }

    void Widget::render(){
        if(visible_ != Normal){
            return;
        }
        onRender();
    }

    Rect Widget::getWidgetRect(){
        const int l = this->left;
        const int t = this->top;
        const int w = this->width_;
        const int h = this->height_;
        return Rect(l, t, w ,h);
    }

    UiRoot* Widget::findRootUi(){
        UiRoot* root = this->rootUi;
        if(root == nullptr && parent_ != nullptr){
            root = this->parent_->findRootUi();
        }
        return root;
    }

    bool Widget::needEatInputBeginEvent(){
        return onClickFn != nullptr;
    }

    void Widget::setSelfUiRootTarget(){
        UiRoot* rootUi = findRootUi();
        if(rootUi != nullptr){
            rootUi->setTargetWidget(this);
        }
    }

    void Widget::setRootUi(UiRoot *root){
        this->rootUi = root;
    }

    void Widget::performClick(InputEvent &event){
        if(onClickFn != nullptr){
            onClickFn(event);
        }
    }
    
    bool Widget::onInputEvent(InputEvent &event){
        bool ret = false;
        const float x = event.x;
        const float y = event.y;

        // std::cout << "widget: " << this->id << " event " << event.action << std::endl;
        
        switch(event.action){
            case EVENT_ACTION_BEGIN:
                if(needEatInputBeginEvent()){
                    ret = true;
                    setSelfUiRootTarget();
                    
                    // if(rect.isPointInRect(x, y)){ 
                    //     ret = true;
                    // }
                }
                break;
            case EVENT_ACTION_MOVE:{
                    auto rect = getWidgetRect();
                    if(!rect.isPointInRect(x, y) && !actionMoveOutScope){
                        actionMoveOutScope = true;
                    }
                }
                break;
            case EVENT_ACTION_END:{
                    auto rect = getWidgetRect();
                    if(!actionMoveOutScope && rect.isPointInRect(x, y)){
                        performClick(event);
                    }
                    actionMoveOutScope = false;
                }
                break;
            case EVENT_ACTION_CANCEL:
                actionMoveOutScope = false;
                break;
            default:
                break;
        }//end switch

        return ret;
    }

    bool Widget::dispatchInputEvent(InputEvent &event){
        if(visible_ != VisibleState::Normal){
            return false;
        }
        
        return onInputEvent(event);
    }
}

