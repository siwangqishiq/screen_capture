#pragma once

enum EventAction{
    ActionDown,
    ActionMove,
    ActionUp
};

class ITouch{
public:
    virtual bool onTouchEvent(EventAction event , float x , float y) = 0;
};