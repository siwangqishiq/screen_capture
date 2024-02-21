#pragma once

#include <cstdint>

class IScreenApi{
public:
    virtual void findScreenSize(int &screenWidth , int &screenHeight) = 0;

    virtual void captureScreen() = 0;
};

class ScreenApi : public IScreenApi{
public:
    ScreenApi(){}

    virtual void findScreenSize(int &screenWidth , int &screenHeight) override;

    virtual void captureScreen() override;

    void savePixel(int w,int h, uint8_t *pixel);

    virtual ~ScreenApi(){}
};