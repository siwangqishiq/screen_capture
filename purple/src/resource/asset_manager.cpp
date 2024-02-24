
#include "resource/asset_manager.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    std::wstring AssetManager::readTextFile(std::string path){
        std::string filePath = assetRootDir() + path;
        Log::i("asset" , "read file path %s" , filePath.c_str());
        return readFileAsWstring(filePath.c_str());
    }

    std::unique_ptr<uint8_t[]> AssetManager::readFileAsBin(std::string path , int &length){
        std::string filePath = assetRootDir() + path;
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

    std::unique_ptr<uint8_t> AssetManager::readTextureFile(std::string path ,
            TextureFileConfig &fileConfig , bool needFlip){
        std::string filePath = assetRootDir() + path;
        Log::i("asset" , "read file path %s" , filePath.c_str());
        
        stbi_set_flip_vertically_on_load(needFlip);

        uint8_t *data = stbi_load(filePath.c_str(),
                            &fileConfig.width , 
                            &fileConfig.height , &fileConfig.channel , 0);
        fileConfig.dataSize = fileConfig.width * fileConfig.height * fileConfig.channel;
        Log::i("asset" , "read file size : %d , %d , %d" , fileConfig.width , fileConfig.height , fileConfig.channel);
        return std::unique_ptr<uint8_t>(data);
    }
}

