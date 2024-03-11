#pragma once

#include <cstdint>

class IScreenApi{
public:
    virtual void findScreenSize(int &screenWidth , int &screenHeight) = 0;

    virtual uint8_t* captureScreen() = 0;
};

class Application;
class ScreenApi : public IScreenApi{
private:
    Application *mApp = nullptr;


public:
    ScreenApi(Application *app): mApp(app){}

    virtual void findScreenSize(int &screenWidth , int &screenHeight) override;

    virtual uint8_t* captureScreen() override;

    void savePixel(int w,int h, uint8_t *pixel);

    void onCaptureFininshed();

    virtual ~ScreenApi(){
    }

};//end class