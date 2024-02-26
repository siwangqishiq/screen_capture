#include "bridge.h"
#include <iostream>
#include <string>
#include "application.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void ScreenApi::onCaptureFininshed(){
    mApp->exit();
}

void ScreenApi::savePixel(int w,int h, uint8_t *pixel){
    std::string filename = "screen.png";
    stbi_write_png(filename.c_str() , w, h, 3 , pixel , 3 * w);
    
    std::cout << "save image finished" << std::endl;

    // onCaptureFininshed();
    // stbi_write_jpg(filename.c_str(), 0, 0, 1, pixel , 100);
    // stbi_write_bmp(filename.c_str() , 0 , 0 ,1 , pixel);
}

//for windows
#ifdef _WIN32
#include <Windows.h>

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

    savePixel(scrWidth , scrHeight , buf);

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

#include <gtk/gtk.h>

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = gdk_screen_get_width(gdk_screen_get_default());
    screenHeight = gdk_screen_get_height(gdk_screen_get_default());
}

uint8_t* ScreenApi::captureScreen(){
    GdkWindow *rootWindow = gdk_get_default_root_window();
    if (rootWindow) {
        GdkPixbuf *screenshot = gdk_pixbuf_get_from_window(root_window, 0, 0, gdk_screen_get_width(gdk_screen_get_default()), gdk_screen_get_height(gdk_screen_get_default()));
        gdk_pixbuf_save(screenshot, "screenshot.png", "png", NULL, NULL);
        g_object_unref(screenshot);

        uint8_t *buf = new uint8_t [3 * scrWidth * scrHeight];

        return buf;
    } 

    return nullptr;
}

// for mac
#elif __APPLE__

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

#else

#endif
