#pragma once

#include "ui/ui.h"
#include "ui/widget.h"
#include <vector>

namespace purple{
    class Container:public Widget{
    public:
        Container(int w , int h):Widget(w, h){
        }
        
        Container():Widget(LAYOUT_MATCH_PARENT, LAYOUT_MATCH_PARENT){
        }
        
        virtual ~Container();

        virtual void addChild(PWidget widget);

        virtual void removeChild(PWidget widget);

        virtual void onMeasure(MeasureSpecMode widthSpecMode, 
                                int widthValue, 
                                MeasureSpecMode heightSpecMode,
                                int heightValue) override;

        virtual void onLayout(int l,int t) override;

        virtual void onRender() override;

        virtual bool dispatchInputEvent(InputEvent &event) override;

        virtual bool isInterceptInputEvent(InputEvent &event);

        void renderContainerSelf();

        std::vector<PWidget>& getChildrenWidgets();
    protected:
        std::vector<PWidget> children_;
    };
}
