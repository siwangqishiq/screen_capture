#pragma once

class IScreenApi{
public:
    virtual void findScreenSize(int &screenWidth , int &screenHeight) = 0;
};

class ScreenApi : public IScreenApi{
public:
    ScreenApi(){}

    virtual void findScreenSize(int &screenWidth , int &screenHeight) override;
    virtual ~ScreenApi(){}
};