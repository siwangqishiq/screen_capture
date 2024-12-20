#pragma once
#include "ui.h"

namespace purple{
    class TextureInfo;
    class UiRoot{
    public:
        UiRoot(int w, int h);
        
        void measure();
        void layout();
        void render();

        void startRenderUI();

        void setRootContainer(PContainer container);

        bool dispatchInputEvent(InputEvent &event);

        void setTargetWidget(Widget *target);

        ~UiRoot();
        
        PContainer rootContainer_ = nullptr;
    private:
        int rootWidth_;
        int rootHeight_;

        std::shared_ptr<TextureInfo> textureInfo_ = nullptr;
        Widget *targetWidget = nullptr;
    };//end class
}
