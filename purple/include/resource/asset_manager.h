
/**
 * @brief  
 * resource access 
 * 
 */
#pragma once

#include <string>
#include <memory>
#include "log.h"
#include "glheader.h"
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
        int dataSize = 0;
        int channel = TEXTURE_FILE_CHANNEL_UNKNOW;
        int format = 0;
    };

    void ConvertChannelToGLConfig(TextureFileConfig &textureFileConfig);

    class AssetManager{
    public:
        static std::shared_ptr<AssetManager> getInstance();
        
        static std::shared_ptr<AssetManager> instance_;

        // read text file
        virtual std::wstring readAssetTextFile(std::string path);

        // read png file 
        virtual std::unique_ptr<uint8_t> readAssetTextureFile(
                        std::string path ,
                        TextureFileConfig &fileConfig ,
                        bool needFlip = false);
        
        virtual int readBinaryFile(std::string path , std::vector<char> &dataVec);

        //二进制方式读取文件 
        virtual std::unique_ptr<uint8_t[]> readFileAsBin(std::string path , int &length);
        
        virtual unsigned char* readAssetFileAsBinRaw(std::string path , int &length);

        virtual long writeFileWithBin(std::string &path , long size , uint8_t *data);

        virtual unsigned char* readFileAsBinRaw(std::string path , int &length);

        inline std::string readAssetTextFileAsString(std::string path){
            return toByteString(readAssetTextFile(path));
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

    public:
        virtual std::string cacheRootDir() const{
            return "cache/";
        }

        std::string ensureCacheDir() const;

        void makedir(std::string dir) const;
    };

    #ifdef __ANDROID__
    #include <android/bitmap.h>
    #include <android/imagedecoder.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>

    class AndroidAssetManager : public AssetManager{
    public:
        static AAssetManager *AndroidAssetManagerInstance;

        virtual std::wstring readAssetTextFile(std::string path) override;

        virtual std::unique_ptr<uint8_t> readAssetTextureFile(std::string path ,
                                                         TextureFileConfig &fileConfig ,
                                                         bool needFlip = false) override;
        //二进制方式读取文件
        virtual unsigned char* readAssetFileAsBinRaw(std::string path ,
                                                        int &length) override;
    private:
        std::unique_ptr<uint8_t> readAssetTextureFileByStbi(std::string path ,TextureFileConfig &fileConfig , bool needFlip);
    };

    #endif
}

