
#include "resource/asset_manager.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

// #include <filesystem>

namespace purple{

    std::shared_ptr<AssetManager> AssetManager::getInstance(){
        static std::shared_ptr<AssetManager> instance_;
        if(instance_ == nullptr){
            #ifdef __ANDROID__
            instance_ = std::make_shared<AndroidAssetManager>();
            #else
            instance_ = std::make_shared<AssetManager>();
            #endif
        }
        return instance_;
    }

    std::wstring AssetManager::readAssetTextFile(std::string path){
        std::string filePath = assetRootDir() + path;
        Log::i("asset" , "read asset file path %s" , filePath.c_str());
        return readFileAsWstring(filePath.c_str());
    }

    unsigned char* AssetManager::readFileAsBinRaw(std::string path , int &length){
        std::string filePath = path;
        Log::i("asset" , "read file path %s" , filePath.c_str());
        try{
            std::ifstream binFile(filePath, std::ios::binary);
            // Logi("asset" , "readBinaryFile is good? %d" , binFile.good);
            
            if(!binFile.is_open()){
                Log::i("asset" , "readBinaryFile is not existed!");
                return nullptr;
            }

            binFile.seekg(0 , std::ios::end);
            length = binFile.tellg();
            binFile.seekg(0 , std::ios::beg);
            Log::i("asset" , "readFileAsBin size %d" , length);
            unsigned char *pData = new unsigned char[length];
            binFile.read((char *)pData , length);
            binFile.close();
            return pData;
        }catch(std::exception &e){
            Log::e("error" , "readBinaryFile code %s" , e.what());
            length = -1;
        }
        return nullptr;
    }

    unsigned char* AssetManager::readAssetFileAsBinRaw(std::string path , int &length){
        std::string filePath = assetRootDir() + path;
        return readFileAsBinRaw(filePath , length);
    }

    std::unique_ptr<uint8_t[]> AssetManager::readFileAsBin(std::string path , int &length){
#ifdef __ANDROID__
        unsigned char* fileData = readAssetFileAsBinRaw(path , length);
        return std::unique_ptr<uint8_t[]>(fileData);
#else

        std::string filePath;
        if(path[0]=='/'){
            filePath = std::string(path.begin() + 1 , path.end());
        }else{
            filePath = assetRootDir() + path;
        }
        Log::i("asset" , "read file path %s" , filePath.c_str());
        try{
            std::ifstream binFile(filePath, std::ios::binary);
            // Logi("asset" , "readBinaryFile is good? %d" , binFile.good);
            
            if(!binFile.is_open()){
                Log::i("asset" , "readBinaryFile is not existed!");
                return nullptr;
            }

            binFile.seekg(0 , std::ios::end);
            
            length = binFile.tellg();
            binFile.seekg(0 , std::ios::beg);

            Log::i("asset" , "readFileAsBin size %d" , length);

            std::unique_ptr<uint8_t[]> pData = std::unique_ptr<uint8_t[]>(new uint8_t[length]);
            binFile.read((char *)pData.get() , length);
            binFile.close();
            
            return pData;
        }catch(std::exception &e){
            Log::e("error" , "readBinaryFile code %s" , e.what());
        }
        return nullptr;
        #endif
    }

    int AssetManager::readBinaryFile(std::string path , std::vector<char> &dataVec){
        std::string filePath = assetRootDir() + path;
        Log::i("asset" , "read file path %s" , filePath.c_str());

        try{
            std::ifstream binFile(filePath, std::ios::binary);
            // Logi("asset" , "readBinaryFile is good? %d" , binFile.good);
            
            if(!binFile.is_open()){
                Log::i("asset" , "readBinaryFile is not existed!");
                return -1;
            }

            binFile.seekg(0 , std::ios::end);
            
            int length = binFile.tellg();
            binFile.seekg(0 , std::ios::beg);

            Log::i("asset" , "readBinaryFile size %d" , length);

            dataVec.resize(length);
            binFile.read(dataVec.data() , length);
            binFile.close();
            
            return length;
        }catch(std::exception &e){
            Log::e("error" , "readBinaryFile code %s" , e.what());
        }
        return -1;
    }

    std::unique_ptr<uint8_t> AssetManager::readAssetTextureFile(std::string path ,
            TextureFileConfig &fileConfig , bool needFlip){
        std::string filePath = assetRootDir() + path;
        Log::i("asset" , "read file path %s" , filePath.c_str());
        
        stbi_set_flip_vertically_on_load(needFlip);
        
        uint8_t *data = stbi_load(filePath.c_str(),
                            &fileConfig.width , 
                            &fileConfig.height ,&fileConfig.channel , 0);
        fileConfig.dataSize = fileConfig.width * fileConfig.height * fileConfig.channel;
        Log::i("asset" , "read file size : %d , %d , %d" , fileConfig.width , fileConfig.height , fileConfig.channel);
        ConvertChannelToGLConfig(fileConfig);

        return std::unique_ptr<uint8_t>(data);
    }

    long AssetManager::writeFileWithBin(std::string &path , long size , uint8_t *data){
        std::fstream file;
        try{
            std::streampos fsize = 0;
            file.open(path , std::ios::out | std::ios::binary);
            fsize = file.tellg();
            file.write(reinterpret_cast<const char*>(data) , size);
            long ret = file.tellg() - fsize;
            file.close();
            return ret;
        }catch(std::exception &e){
            if(file.is_open()){
                file.close();
            }
            return -1;
        }
        return 0;
    }

    std::string AssetManager::ensureCacheDir() const{
        std::string dir = cacheRootDir();
        makedir(dir);
        return dir;
    }

    void AssetManager::makedir(std::string dir) const{
        #ifdef _WIN32
        mkdir(dir.c_str());
        #else
        int err = mkdir(dir.c_str() , 0x777);
        if(err != 0){
            perror("mkdir");
        }
        #endif
    }

    void ConvertChannelToGLConfig(TextureFileConfig &textureFileConfig){
        textureFileConfig.format = GL_RGBA;
        switch (textureFileConfig.channel) {
            case TEXTURE_FILE_CHANNEL_UNKNOW:
                textureFileConfig.format = GL_RGBA;
                break;
            case TEXTURE_FILE_CHANNEL_R:
                textureFileConfig.format = GL_RED;
                break;
            case TEXTURE_FILE_CHANNEL_RGB:
                textureFileConfig.format = GL_RGB;
                break;
            case TEXTURE_FILE_CHANNEL_ARGB:
            case TEXTURE_FILE_CHANNEL_RGBA:
                textureFileConfig.format = GL_RGBA;
                break;
        }//end switch
    }

    #ifdef __ANDROID__

    AAssetManager *AndroidAssetManager::AndroidAssetManagerInstance = nullptr;

    std::wstring AndroidAssetManager::readAssetTextFile(std::string path){
         std::string filePath = path;
        AAsset *file = AAssetManager_open(AndroidAssetManagerInstance , filePath.c_str(), AASSET_MODE_BUFFER);
        if(file == nullptr){
            Log::i("asset","open asset file : %s failed" , filePath.c_str());
            return L"";
        }

        Log::i("asset" , "get asset file");
        size_t fileSize = AAsset_getLength(file);

        auto fileContentBuf = std::unique_ptr<char []>(new char[fileSize + 1]);
        AAsset_read(file , fileContentBuf.get() , fileSize);
        fileContentBuf[fileSize] = '\0';
        AAsset_close(file);

//        Log::i("asset_manager","file content = %s" , fileContentBuf.get());
        std::wstring contentStr = toWideString(std::string(fileContentBuf.get()));
        return contentStr;
    }

    unsigned char* AndroidAssetManager::readAssetFileAsBinRaw(
                        std::string path ,
                        int &length){
        Log::i("android_read_asset","read android file asset : %s" , path.c_str());
        auto assetFile  = AAssetManager_open(AndroidAssetManagerInstance , path.c_str() , AASSET_MODE_STREAMING);
        size_t fileSize = AAsset_getLength(assetFile);
        unsigned char *contentBuf = new unsigned char[fileSize];
        AAsset_read(assetFile, contentBuf , fileSize);
        AAsset_close(assetFile);
        Log::i("android_read_asset","read android file size : %d" , fileSize);
        length = fileSize;
        return contentBuf;
    }

    std::unique_ptr<uint8_t>
    AndroidAssetManager::readAssetTextureFile(
            std::string path,
            TextureFileConfig &fileConfig,
            bool needFlip){
        return readAssetTextureFileByStbi(path , fileConfig , needFlip);
    }

    std::unique_ptr<uint8_t> AndroidAssetManager::readAssetTextureFileByStbi(std::string path ,TextureFileConfig &fileConfig , bool needFlip){
        Log::i("asset" , "decode image using stbi library");
        auto assetFile  = AAssetManager_open(AndroidAssetManagerInstance , path.c_str() , AASSET_MODE_STREAMING);
        size_t fileSize = AAsset_getLength(assetFile);
        auto contentBuf = std::unique_ptr<char []>(new char[fileSize]);
        AAsset_read(assetFile, contentBuf.get() , fileSize);
        AAsset_close(assetFile);

        stbi_set_flip_vertically_on_load(needFlip);
        uint8_t *data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>
                                                (contentBuf.get()), fileSize , &fileConfig.width ,
                                              &fileConfig.height , &fileConfig.channel, 0);
        fileConfig.dataSize = fileConfig.width * fileConfig.height * fileConfig.channel;
        ConvertChannelToGLConfig(fileConfig);
        Log::i("android_read_asset","read file %s , width %d  height %d  channel: %d" ,
                path.c_str() , fileConfig.width , fileConfig.height , fileConfig.channel);
        return std::unique_ptr<uint8_t>(data);
    }

    #endif
}

