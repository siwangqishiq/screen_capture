#include "ui/stack_container.h"

namespace purple{
    StackContainer::~StackContainer(){
        Log::i("ui","StackContainer descontructor");
    }

    void StackContainer::onMeasure(MeasureSpecMode widthSpecMode, 
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue){
        int limitMaxWidth = 0;
        if(widthSpecMode == MeasureSpecMode::Exactly){
            setWidth(widthValue);
            limitMaxWidth = widthValue;
        }else if(widthSpecMode == MeasureSpecMode::Atmost) {
            setWidth(LAYOUT_UNSET);
            limitMaxWidth = widthValue;
        }else{
            setWidth(LAYOUT_UNSET);
            limitMaxWidth = WIDGET_MAX_WIDTH;
        }

        int limitMaxHeight = 0 ;
        if(heightSpecMode == MeasureSpecMode::Exactly){
            setHeight(heightValue);
            limitMaxHeight = heightValue;
        } else if(heightSpecMode == MeasureSpecMode::Atmost){
            setWidth(LAYOUT_UNSET);
            limitMaxHeight = heightValue;
        } else {
            setHeight(LAYOUT_UNSET);
            limitMaxHeight = WIDGET_MAX_HEIGHT;
        }

        childMaxWidth = 0;
        childMaxHeight = 0;

        measureChildWidgets(limitMaxWidth , limitMaxHeight);

        bool needRemeasure = false;
        if(widthSpecMode != MeasureSpecMode::Exactly){
            setWidth(childMaxWidth + getPaddingHorizontal());
            needRemeasure = true;
        }
        
        if(heightSpecMode != MeasureSpecMode::Exactly){
            // std::cout << "stack container set height = " << childMaxHeight + getPaddingVertial() << std::endl;
            setHeight(childMaxHeight + getPaddingVertial());
            needRemeasure = true;
        }

        if(needRemeasure){
            measureChildWidgets(getWidth() , getHeight());
        }

        // std::cout << "stack contaier " << this->id <<" width = " 
        //     << width_ << "  height: " << height_ << std::endl;
    }

    void StackContainer::measureChildWidgets(int parentLimitWidth , int parentLimitHeight){
        for(auto &pWidget : getChildrenWidgets()){
            if(pWidget == nullptr){
                continue;
            }

            //width set
            MeasureSpecMode widthMode = MeasureSpecMode::Unset;
            int widthValue = 0;
            if(pWidget->getRequestWidth() == LAYOUT_MATCH_PARENT){
                widthMode = MeasureSpecMode::Exactly;
                widthValue = std::max(0, parentLimitWidth);
            }else if(pWidget->getRequestWidth() == LAYOUT_WRAP_CONTENT){
                widthMode = MeasureSpecMode::Atmost;
                widthValue = parentLimitWidth;
            }else{
                widthMode = MeasureSpecMode::Exactly;
                widthValue = std::min(pWidget->getRequestWidth() , parentLimitWidth);
            }

            //height set
            MeasureSpecMode heightMode = MeasureSpecMode::Unset;
            int heightValue = 0;
            if(pWidget->getRequestHeight() == LAYOUT_MATCH_PARENT){
                heightMode = MeasureSpecMode::Exactly;
                heightValue = std::max(0, parentLimitHeight);
            }else if(pWidget->getRequestHeight() == LAYOUT_WRAP_CONTENT){
                heightMode = MeasureSpecMode::Atmost;
                heightValue = parentLimitHeight;
            }else{
                heightMode = MeasureSpecMode::Exactly;
                heightValue = std::min(pWidget->getRequestHeight() , parentLimitHeight);
            }

            pWidget->measure(widthMode , widthValue , heightMode , heightValue);

            // std::cout << "pWidget->measure id = " << pWidget->id << "  mode = " 
            //     << heightMode
            //     << " height = " << pWidget->getHeight() <<std::endl;

            if(childMaxWidth < pWidget->getWidth() + pWidget->getMarginHorizontal()){
                childMaxWidth = pWidget->getWidth() + pWidget->getMarginHorizontal();
            }

            if(childMaxHeight < pWidget->getHeight() + pWidget->getMarginVertical()){
                childMaxHeight = pWidget->getHeight() + pWidget->getMarginVertical();
            }
        }//end for each    
    }

    void StackContainer::onLayout(int l,int t){
        this->Container::onLayout(l , t);

        for(PWidget widget: getChildrenWidgets()){
            if(widget == nullptr || widget->getVisible() == Gone){
                continue;
            }

            switch(widget->getLayoutGravity()){
                case TopLeft:
                    widget->layout(
                        paddingLeft_ + left + widget->getMarginLeft(), 
                        top - paddingTop_ - widget->getMarginTop()
                    );
                    break;
                case TopCenter:
                    widget->layout(
                        left + (getWidth() >> 1) - (widget->getWidth() >> 1), 
                        top - paddingTop_ - widget->getMarginTop()
                    );
                    break;
                case TopRight:
                    widget->layout(
                        left + getWidth() - paddingRight_ - widget->getWidth() - widget->getMarginRight(), 
                        top - paddingTop_ - widget->getMarginTop()
                    );
                    break;
                case CenterLeft:
                    widget->layout(
                        paddingLeft_ + left + widget->getMarginLeft(), 
                        top - (getHeight() >> 1) + (widget->getHeight() >> 1)
                    );
                    break;
                case Center:
                    widget->layout(
                        left + (getWidth() >> 1) - (widget->getWidth() >> 1), 
                        top - (getHeight() >> 1) + (widget->getHeight() >> 1)
                    );
                    break;
                case CenterRight:
                    widget->layout(
                        left + getWidth() - paddingRight_ - widget->getMarginRight() - widget->getWidth(), 
                        top - (getHeight() >> 1) + (widget->getHeight() >> 1)
                    );
                    break;
                case BottomLeft:
                    widget->layout(
                        left + paddingLeft_ + widget->getMarginLeft(),
                        top - getHeight() + paddingBottom_ + widget->getMarginBottom() + widget->getHeight()
                    );
                    break;
                case BottomCenter:
                    widget->layout(
                        left + (getWidth() >> 1) - (widget->getWidth() >> 1),
                        top - getHeight() + paddingBottom_ + widget->getMarginBottom() + widget->getHeight()
                    );
                    break;
                case BottomRight:
                    widget->layout(
                        left + getWidth() - paddingRight_ - widget->getMarginRight() - widget->getWidth(),
                        top - getHeight() + paddingBottom_ + widget->getMarginBottom() + widget->getHeight()
                    );
                    break;
                default:
                    break;
            }//end switch
        }//end for each
    }
}