#pragma once

#include "ui.h"
#include "ui/container.h"
#include "color.h"
#include "render/common.h"

namespace purple{

    class RowContainer : public Container{
    public:
        RowContainer():Container(LAYOUT_MATCH_PARENT, LAYOUT_WRAP_CONTENT){

        }

        RowContainer(int reqWidth,int reqHeight):Container(reqWidth, reqHeight){

        }

        virtual void onMeasure(MeasureSpecMode widthSpecMode, 
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue) override;
        
        virtual void onLayout(int l,int t) override;
        virtual ~RowContainer();

    private:
        int childWidgetMaxHeight = 0;
        int childWidgetTotalWidth = 0;

        std::vector<PWidget> hasWeightWidgets; 

        void measureChildWidgets(int limitMaxWdith , int limitMaxHeight);

        void measureChildWidth(PWidget widget , 
                                MeasureSpecMode &outWidthMode , 
                                int &outWidthValue ,
                                int limitMaxWdith);
        
        void measureChildHeight(PWidget widget, 
                                MeasureSpecMode &outHeightMode , 
                                int &outHeightValue ,
                                int limitMaxHeight);
                                
        void measureWeightWidgets(int limitMaxWidth , int limitMaxHeight);
    };
}