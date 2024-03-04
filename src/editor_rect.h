

#pragma once

#include "editor.h"
#include "application.h"

class RectEditor : public IEditor{
public:
    RectEditor(Application *_app) : app(_app){}

    virtual void renderEditorContent() override;

    virtual bool dispatchEventAction(EventAction action , float x , float y) override;
private:
    Application *app;

    float startX;
    float startY;

};
