#include "input/input_manager.h"
#include "log.h"

namespace purple{
    std::unique_ptr<InputManager> InputManager::instance_ = nullptr;
    std::mutex InputManager::mtx;

    InputManager::InputManager(){
        Log::i("purple","InputManager construct");
    }

    InputManager::~InputManager(){
        clearCallback();
        Log::i("input_manager","InputManager deconstructed");
    }

    std::unique_ptr<InputManager>& InputManager::getInstance(){
        if(instance_ == nullptr){
            std::lock_guard<std::mutex> lock(mtx);
            instance_ = std::make_unique<InputManager>();
        }
        return instance_;
    }

    void InputManager::onEvent(InputEvent event){
//         Log::i("input" , "on event action : %d (%f , %f)" ,event.action , event.x , event.y);

        for(auto &pair : onEventCallbacks){
            if(pair.second != nullptr){
                bool cost = pair.second(event);
                if(cost){
                    break;
                }
            }
        }//end for each
    }

    int InputManager::getEventListenCallbackCount(){
        return onEventCallbacks.size();
    }

    void InputManager::clearCallback(){
        onEventCallbacks.clear();
    }

    void InputManager::addEventListener(std::string callbackId, OnEventCallback callback){
        onEventCallbacks[callbackId] = callback;
    }

    void InputManager::removeEventListener(std::string callbackId){
        onEventCallbacks.erase(callbackId);
    }
}
