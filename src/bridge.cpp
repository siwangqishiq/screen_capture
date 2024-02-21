#include "bridge.h"
#include <iostream>
#include <string>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void ScreenApi::savePixel(int w,int h, uint8_t *pixel){
    std::string filename = "screen_capture.png";
    stbi_write_png(filename.c_str() , w, h, 1 , pixel , 0);
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

void ScreenApi::captureScreen(){
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
    HBITMAP holdbmp = static_cast<HBITMAP>(SelectObject(hmdc, hBmpScreen));

    BITMAP bm;
    GetObject(hBmpScreen, sizeof(bm), &bm);

    BITMAPINFOHEADER bi = { 0 };
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = scrWidth;
    bi.biHeight = -scrHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    uint32_t  *buf = new uint32_t [scrWidth*scrHeight];
    BitBlt(hmdc, 0, 0, scrWidth, scrHeight, hdc, rect.left, rect.top, SRCCOPY);
    GetDIBits(hmdc, hBmpScreen, 0L, (DWORD)scrHeight, buf, (LPBITMAPINFO)&bi, (DWORD)DIB_RGB_COLORS);

    savePixel(scrWidth , scrHeight , (uint8_t *)buf);
    delete[] buf;
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

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

// for mac
#elif __APPLE__

void ScreenApi::findScreenSize(int &screenWidth , int &screenHeight){
    screenWidth = 1280;
    screenHeight = 800;
}

#else

#endif
