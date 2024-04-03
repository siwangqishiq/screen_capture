#pragma once

enum EventAction{
    ActionDown,
    ActionMove,
    ActionUp,
    CursorMove //代表鼠标移动   但是可能并未被按下
};

class ITouch{
public:
    virtual bool onTouchEvent(EventAction event , float x , float y) = 0;
};