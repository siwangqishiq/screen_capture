#pragma once

#include "ui.h"
#include "ui/container.h"
#include "color.h"
#include "render/common.h"


namespace purple{
    class ColumContainer : public Container{
    public:
        ColumContainer(int w , int h):Container(w, h){
        }
        
        ColumContainer():Container(LAYOUT_MATCH_PARENT, LAYOUT_MATCH_PARENT){
        }

        virtual ~ColumContainer();

        virtual void onMeasure(MeasureSpecMode widthSpecMode,
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue) override;

        virtual void onLayout(int l,int t) override;

    private:
        int chidlWidgetMaxWidth = 0;
        int childWidgetTotalHeight = 0;

        std::vector<PWidget> hasWeightWidgets_;

        void measureChildWidgets(int limitMaxWidth, int limitMaxHeight);
        
        void measureChildWidth(PWidget child, int limitMaxwidth, MeasureSpecMode &mode , int &value);
        void measureChildHeight(PWidget child,int limitMaxHeight, MeasureSpecMode &mode , int &value);

        void measureWeightWidgets(int limitMaxWidth , int limitMaxHeight);
    };
}