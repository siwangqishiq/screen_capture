#pragma once

#include <iostream>

namespace purple{
    const int EVENT_ACTION_BEGIN = 1;
    const int EVENT_ACTION_MOVE = 2;
    const int EVENT_ACTION_END = 3;
    const int EVENT_ACTION_CANCEL = 4;

    const int EVENT_ACTION_MOUSE_MIDDLE_BEGIN = 101;
    const int EVENT_ACTION_MOUSE_MIDDLE_MOVE = 102;
    const int EVENT_ACTION_MOUSE_MIDDLE_END = 103;

    const int EVENT_ACTION_MOUSE_RIGHT_BEGIN = 201;
    const int EVENT_ACTION_MOUSE_RIGHT_MOVE = 202;
    const int EVENT_ACTION_MOUSE_RIGHT_END = 203;

    const int EVENT_ACTION_KEY = 10;//键盘按下事件

    struct InputEvent{
        int action;
        float x;
        float y;
        int code;
        void *ptrParam;
    };
}
