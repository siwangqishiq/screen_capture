#include "platform/bridge.h"
#include <iostream>
#include <string>
#include "application.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"

void ScreenApi::onCaptureFininshed(){
    mApp->exit();
}

void ScreenApi::savePixel(int w,int h, uint8_t *pixel){
    // std::string filename = "screen.png";
    // stbi_write_png(filename.c_str() , w, h, 3 , pixel , 3 * w);
    // std::cout << "save image finished" << std::endl;
    // onCaptureFininshed();
    // stbi_write_jpg(filename.c_str(), 0, 0, 1, pixel , 100);
    // stbi_write_bmp(filename.c_str() , 0 , 0 ,1 , pixel);
}

//for windows
#ifdef _WIN32
#include <Windows.h>

bool Application::isWindows = true;
bool Application::isMac = false;
bool Application::isLinux = false;

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
}

uint8_t* ScreenApi::captureScreen(){
    // std::string filename = "test.bmp";
    
    HDC hdc = CreateDC("DISPLAY" , nullptr , nullptr , nullptr);
    int scrWidth = 0;
    int scrHeight = 0;
    RECT rect = {0};
    scrWidth = GetDeviceCaps(hdc, HORZRES);
    scrHeight = GetDeviceCaps(hdc, VERTRES);

    std::cout << "screen size " << scrWidth << " ," << scrHeight << std::endl;

    HDC hmdc = CreateCompatibleDC(hdc);
    HBITMAP hBmpScreen = CreateCompatibleBitmap(hdc , scrWidth , scrHeight);
    SelectObject(hmdc, hBmpScreen);
    // HBITMAP holdbmp = static_cast<HBITMAP>(SelectObject(hmdc, hBmpScreen));
    
    BITMAP bm;
    GetObject(hBmpScreen, sizeof(bm), &bm);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = scrWidth;
    bi.biHeight = scrHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 8 * 3;
    bi.biCompression = BI_RGB;

    uint8_t  *buf = new uint8_t [3 * scrWidth * scrHeight];
    BitBlt(hmdc, 0, 0, scrWidth, scrHeight, hdc, rect.left, rect.top, SRCCOPY);
    GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)scrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

    // bgr -> rgb
    const int size = scrWidth * scrHeight * 3;
    for(int i = 0 ; i < size ;i+=3){
        auto tmp = buf[i];
        buf[i] = buf[i + 2];
        buf[i + 2] = tmp;
    }//end for i

    // savePixel(scrWidth , scrHeight , buf);

    return buf;

    // delete[] buf;
    // BITMAPFILEHEADER bfh = { 0 };
    // bfh.bfType = ((WORD)('M' << 8) | 'B');
    // bfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bi.biSizeImage;
    // bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    // HANDLE hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    // DWORD dwWrite;
    // WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    // WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
    // WriteFile(hFile, buf, bi.biSizeImage, &dwWrite, NULL);
    // CloseHandle(hFile);
    // hBmpScreen = (HBITMAP)SelectObject(hmdc, holdbmp);
}

// for linux
#elif __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

bool Application::isWindows = false;
bool Application::isMac = false;
bool Application::isLinux = true;

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    Display *display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes winAttr;
    XGetWindowAttributes(display , root , &winAttr);
    screenWidth = winAttr.width;
    screenHeight = winAttr.height;
}

uint8_t* ScreenApi::captureScreen(){
    int scrWidth = 0;
    int scrHeight = 0;
    findScreenSize(scrWidth , scrHeight);

    uint8_t *pixelBuf = new uint8_t [3 * scrWidth * scrHeight];

    Display *display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XImage *image = XGetImage(display, root , 0, 0, 
        scrWidth , scrHeight, AllPlanes , ZPixmap);

    int index = 0;
    for(int y = scrHeight - 1; y >= 0 ;y--){
        for(int x = 0; x < scrWidth ; x++){
            unsigned long pixel = XGetPixel(image, x, y);
            uint8_t red = pixel >> 16;
            uint8_t green = (pixel >> 8) & 0xff;
            uint8_t blue = (pixel)& 0xff;

            pixelBuf[index + 0] = red;
            pixelBuf[index + 1] = green;
            pixelBuf[index + 2] = blue;

            index += 3;
        }//end for x
    }//end for y
    return pixelBuf;
}

// for mac
#elif __APPLE__

bool Application::isWindows = false;
bool Application::isMac = true;
bool Application::isLinux = false;

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

uint8_t* ScreenApi::captureScreen(){
    int scrWidth = 0;
    int scrHeight = 0;
    findScreenSize(scrWidth , scrHeight);

    uint8_t *pixelBuf = new uint8_t [3 * scrWidth * scrHeight];
    return pixelBuf;
}

#else //other platforms raspiberry playstation switch xbox?

#endif
