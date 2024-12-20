#include "ui/row_container.h"
#include <algorithm>

namespace purple{

    void RowContainer::onMeasure(MeasureSpecMode widthSpecMode, 
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue) {
        childWidgetMaxHeight = 0;
        childWidgetTotalWidth = 0;
        hasWeightWidgets.clear();

        int limitMaxWidth = WIDGET_MAX_WIDTH;
        setWidth(LAYOUT_UNSET);
        if(widthSpecMode == MeasureSpecMode::Exactly){
            limitMaxWidth = widthValue;
            setWidth(widthValue);
        }else if(widthSpecMode == MeasureSpecMode::Atmost){
            limitMaxWidth = widthValue;
        }

        int limitMaxHeight = WIDGET_MAX_HEIGHT;
        setHeight(LAYOUT_UNSET);
        if(heightSpecMode == MeasureSpecMode::Exactly){
            limitMaxHeight = heightValue;
            setHeight(heightValue);
        }else if(heightSpecMode == MeasureSpecMode::Atmost){
            limitMaxHeight = heightValue;
        }

        // std::cout << "limitMaxHeight = " << limitMaxHeight << "  id = " << this->id << std::endl;
        measureChildWidgets(limitMaxWidth, limitMaxHeight);
        
        bool needReMeasure = false;
        if(widthSpecMode != MeasureSpecMode::Exactly){
            setWidth(std::min(limitMaxWidth , childWidgetTotalWidth + getPaddingHorizontal()));
            needReMeasure = true;
        }
        if(heightSpecMode != MeasureSpecMode::Exactly){
            setHeight(std::min(limitMaxHeight , childWidgetMaxHeight + getPaddingVertial()));
            needReMeasure = true;
        }
        
        if(needReMeasure){
            childWidgetTotalWidth = 0;
            measureChildWidgets(getWidth(), getHeight());
        }

        // std::cout << "stack contaier " << this->id <<" width = " 
        //     << width_ << "  height: " << height_ << std::endl;
    }

    void RowContainer::measureChildWidgets(int limitMaxWdith , int limitMaxHeight){
        for(auto pWidget : getChildrenWidgets()){
            if(pWidget == nullptr){
                continue;
            }
            
            MeasureSpecMode widthMode = MeasureSpecMode::Unset;
            int widthValue = 0;
            measureChildWidth(pWidget , widthMode , widthValue , limitMaxWdith);

            MeasureSpecMode heightMode = MeasureSpecMode::Unset;
            int heightValue = 0;
            measureChildHeight(pWidget , heightMode , heightValue , limitMaxHeight);

            pWidget->measure(widthMode , widthValue , heightMode , heightValue);

            if(widthMode != MeasureSpecMode::Unset){
                const int costWidth = pWidget->getWidth() + pWidget->getMarginHorizontal();
                childWidgetTotalWidth += costWidth;
            }
            
            if(heightMode != MeasureSpecMode::Unset) {
                const int costHeight = pWidget->getHeight() + pWidget->getMarginVertical();
                // std::cout << "id = " << pWidget->id << "  costHeight = " << costHeight << std::endl;
                if(childWidgetMaxHeight < costHeight) {
                    childWidgetMaxHeight = costHeight;
                }
            }
        }//end for each

        measureWeightWidgets(limitMaxWdith, limitMaxHeight);
    }

    void RowContainer::measureWeightWidgets(int limitMaxWidth , int limitMaxHeight){
        if(hasWeightWidgets.empty()){
            return;
        }

        int totalWeightValue = 0;
        int weightWidgetMargins = 0;
        for(auto &pWidget : hasWeightWidgets){
            if(pWidget->getVisible() != VisibleState::Normal){
                continue;
            }

            totalWeightValue += pWidget->getLayoutWeight();
            weightWidgetMargins += pWidget->getMarginHorizontal();
        }//end for each

        const int remainWidth = getContentWidth() - childWidgetTotalWidth - weightWidgetMargins;
        // std::cout << "getWidth() = " << getWidth() << "   padding = " <<
        //     getPaddingHorizontal() << std::endl;
        // std::cout << "RowContainer remainWidth = " 
        //     << getContentWidth() - childWidgetTotalWidth << std::endl;

        if(remainWidth > 0 && totalWeightValue > 0){
            const int weightCube = remainWidth / totalWeightValue;
            for(auto &pWidget : hasWeightWidgets){
                MeasureSpecMode widthMode = MeasureSpecMode::Exactly;
                int widthValue = weightCube * pWidget->getLayoutWeight();

                MeasureSpecMode heightMode = MeasureSpecMode::Unset;
                int heightValue = 0;
                measureChildHeight(pWidget , heightMode , heightValue , limitMaxHeight);

                pWidget->measure(widthMode , widthValue , heightMode , heightValue);
            }//end for each
        }

        hasWeightWidgets.clear();
    }

    void RowContainer::measureChildWidth(PWidget widget, 
                MeasureSpecMode &outWidthMode, 
                int &outWidthValue,
                int limitMaxWdith) {
        const int reqWidth = widget->getRequestWidth();
        if(limitMaxWdith == LAYOUT_UNSET){ //父宽度未知
            if(reqWidth == LAYOUT_MATCH_PARENT){
                outWidthMode = MeasureSpecMode::Atmost;
                outWidthValue = WIDGET_MAX_WIDTH;
            }else if(reqWidth == LAYOUT_WRAP_CONTENT){
                outWidthMode = MeasureSpecMode::Atmost;
                outWidthValue = WIDGET_MAX_WIDTH;
            }else if(reqWidth == LAYOUT_UNSET){
                outWidthMode = MeasureSpecMode::Exactly;
                outWidthValue = 0;
            }else{
                outWidthMode = MeasureSpecMode::Exactly;
                outWidthValue = reqWidth;
            }
        }else{ //父宽度已知
            if(reqWidth == LAYOUT_MATCH_PARENT){
                outWidthMode = MeasureSpecMode::Exactly;
                outWidthValue = limitMaxWdith - getPaddingHorizontal() - widget->getMarginHorizontal();
            }else if(reqWidth == LAYOUT_WRAP_CONTENT){
                outWidthMode = MeasureSpecMode::Atmost;
                outWidthValue = limitMaxWdith - getPaddingHorizontal() - widget->getMarginHorizontal();
            }else if(reqWidth == LAYOUT_UNSET){
                outWidthMode = MeasureSpecMode::Exactly;
                outWidthValue = 0;
                if(widget->getLayoutWeight() > 0){
                    outWidthMode = MeasureSpecMode::Unset;
                    outWidthValue = 0;
                    hasWeightWidgets.emplace_back(widget);
                }
            }else{
                outWidthMode = MeasureSpecMode::Exactly;
                outWidthValue = reqWidth;
            }
        }
    }

    void RowContainer::measureChildHeight(PWidget widget, 
                            MeasureSpecMode &outHeightMode , 
                            int &outHeightValue,
                            int limitMaxHeight){
        const int reqHeight = widget->getRequestHeight();
        if(limitMaxHeight == LAYOUT_UNSET){//父控件高度未知
            if(reqHeight == LAYOUT_MATCH_PARENT){
                outHeightMode = MeasureSpecMode::Atmost;
                outHeightValue = WIDGET_MAX_HEIGHT;
            }else if(reqHeight == LAYOUT_WRAP_CONTENT){
                outHeightMode = MeasureSpecMode::Atmost;
                outHeightValue = WIDGET_MAX_HEIGHT;
            }else if(reqHeight == LAYOUT_UNSET){
                outHeightMode = MeasureSpecMode::Exactly;
                outHeightValue = 0;
            }else {
                outHeightMode = MeasureSpecMode::Exactly;
                outHeightValue = reqHeight;
            }
        }else{ //父控件高度已知
            if(reqHeight == LAYOUT_MATCH_PARENT){
                outHeightMode = MeasureSpecMode::Exactly;
                outHeightValue = limitMaxHeight - getPaddingVertial() - widget->getMarginVertical();
            }else if(reqHeight == LAYOUT_WRAP_CONTENT){
                outHeightMode = MeasureSpecMode::Atmost;
                outHeightValue = limitMaxHeight - getPaddingVertial() - widget->getMarginVertical();
            }else if(reqHeight == LAYOUT_UNSET){
                outHeightMode = MeasureSpecMode::Exactly;
                outHeightValue = 0;
            }else {
                outHeightMode = MeasureSpecMode::Exactly;
                outHeightValue = reqHeight;
            }
        }//end if
    }

    void RowContainer::onLayout(int l,int t) {
        this->Widget::onLayout(l , t);

        int x = paddingLeft_ + left;
        int y = top - paddingTop_;

        for(auto widget: getChildrenWidgets()){
            if(widget == nullptr || widget->getVisible() == Gone){
                continue;
            }
            
            const LayoutGravity gravity = widget->getLayoutGravity();
            if(gravity == LayoutGravity::TopCenter
                || gravity == LayoutGravity::TopLeft
                || gravity == LayoutGravity::TopRight){ // top
                y = top - paddingTop_;
            }else if(gravity == LayoutGravity::CenterLeft 
                || gravity == LayoutGravity::Center 
                || gravity == LayoutGravity::CenterRight){ // center
                y = top - (this->height_ >> 1) + (widget->getHeight() >> 1);
            }else{ //bottom
                y = top - this->height_ + widget->getHeight();
            }//end if
            
            widget->layout(x + widget->getMarginLeft() , y - widget->getMarginTop()); 
            x += widget->getMarginLeft() + widget->getMarginRight() + widget->getWidth();
        }//end for each
    }

    RowContainer::~RowContainer(){
        
    }
}