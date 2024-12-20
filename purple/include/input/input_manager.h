//
// 用户输入
//
#pragma once

#include <memory>
#include <mutex>
#include <functional>
#include <map>
#include <algorithm>
#include <string>
#include "input/input_common.h"

namespace purple{

    typedef std::function<bool(InputEvent&)> OnEventCallback;

    class InputManager{
    public:
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;    

        static std::unique_ptr<InputManager>& getInstance();

        InputManager();
        ~InputManager();

        bool isMouseLeftPressed = false;
        bool isMouseMiddlePressed = false;
        bool isMouseRightPressed = false;

        void onEvent(InputEvent event);

        void addEventListener(std::string callbackId, OnEventCallback callback);

        void removeEventListener(std::string callbackId);

        int getEventListenCallbackCount();

        void clearCallback();

    private:
        static std::mutex mtx;
        static std::unique_ptr<InputManager> instance_;

        bool isKeyBoardPressed = false;

        std::map<std::string, OnEventCallback> onEventCallbacks;
    };//end class InputManager
}

