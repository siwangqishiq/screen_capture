
/**
 * @brief  
 * resource access 
 * 
 */
#pragma once

#include <string>
#include <memory>
#include "log.h"
#include <fstream>
#include <streambuf>
#include <codecvt>
#include <vector>

namespace purple{
    const int TEXTURE_FILE_CHANNEL_UNKNOW = -1;
    const int TEXTURE_FILE_CHANNEL_R = 1;
    const int TEXTURE_FILE_CHANNEL_RGB = 3;
    const int TEXTURE_FILE_CHANNEL_ARGB = 4;
    const int TEXTURE_FILE_CHANNEL_RGBA = 5;

    struct TextureFileConfig{
        int width = 0;
        int height = 0;
        long dataSize = 0;
        int channel = TEXTURE_FILE_CHANNEL_UNKNOW;
    };

    class AssetManager{
    public:
        static std::shared_ptr<AssetManager> getInstance();

        static std::shared_ptr<AssetManager> instance_;

        // read text file
        virtual std::wstring readTextFile(std::string path);

        // read png file 
        virtual std::unique_ptr<uint8_t> readTextureFile(std::string path ,
                    TextureFileConfig &fileConfig , bool needFlip = false);
        
        virtual int readBinaryFile(std::string path , std::vector<char> &dataVec);

        //二进制方式读取文件 
        virtual std::unique_ptr<uint8_t[]> readFileAsBin(std::string path , int &length);
        
        inline std::string readTextFileAsString(std::string path){
            return toByteString(readTextFile(path));
        }

        AssetManager(){
            Log::i("asset_manager" , "asset manager construct");
        }

        ~AssetManager(){
            Log::i("asset_manager" , "~asset manager deconstruct");
        }

    protected:
        std::string assetRootDir() {
            return "../assets/";
        }

    protected:
        inline std::wstring toWideString(const std::string& input){
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.from_bytes(input);
        }

        inline std::string toByteString(const std::wstring& input){
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.to_bytes(input);
        }

        inline std::wstring readFileAsWstring(const char *path){
            std::ifstream file(path);
            std::string str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
            return toWideString(str);
        }
    };

}

