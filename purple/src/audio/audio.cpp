
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "audio/audio.h"
#include "log.h"
#include "resource/asset_manager.h"
#include <functional>

namespace purple{

    struct AudioEntityUserData{
        AudioEntity *entity;
    };

    class AudioEntity{
    public:
        std::string name;
        std::unique_ptr<uint8_t[]> pData;
        ma_device device;
        ma_device_config deviceConfig;
        ma_device_data_proc dataCallback;

        AudioEntityUserData userData;

        ma_decoder decoder;
        bool isPlay = false;
        bool isLoop = false;
        bool isReleased = false;
        unsigned long pcmFrameLength = 0;
        double duration = 0.0f;
        unsigned long readedFrame = 0;//记录播放时

         //播放进度更新回调
        ProgressUpdateCallbackFn onPlayProgressUpdate = nullptr;

        //播放结束 回调
        PlayEndCallbackFn onPlayEndCallback = nullptr;

        void setPlayProgressUpdateCallback(ProgressUpdateCallbackFn callback){
            onPlayProgressUpdate = callback;
        }

        void setPlayEndCallback(PlayEndCallbackFn callback){
            onPlayEndCallback = callback;
        }
    };

    std::shared_ptr<AudioManager> AudioManager::getInstance(){
        static std::shared_ptr<AudioManager> instance_;
        if(instance_ == nullptr){
            instance_ = std::make_shared<AudioManager>();
        }
        return instance_;
    }

    std::shared_ptr<AudioEntity> AudioManager::loadAudioEntity(std::string path , bool playLoop){
        // std::vector<char> dataVec;
        // AssetManager::getInstance()->readBinaryFile(path , dataVec);
        // Logi("audio" , "loadAudioResource file size: %d" , dataVec.size());
        
        int fileSize = 0;
        auto fileData = AssetManager::getInstance()->readFileAsBin(path , fileSize);
        Log::i("audio" , "loadAudioResource file size: %d" , fileSize);
        if(fileSize <= 0){
            Log::i("audio" , "loadAudioResource read file %s failed" , path.c_str());
        }
        
        std::shared_ptr<AudioEntity> entity = std::make_shared<AudioEntity>();
        entity->name = path;
        entity->pData.reset(fileData.release());
        entity->isLoop = playLoop;

        if(ma_decoder_init_memory(entity->pData.get() , 
                fileSize , nullptr , &entity->decoder) != MA_SUCCESS){
            Log::i("audio" , "loadAudioResource %s create decoder error" , path.c_str());
            return nullptr;
        }

        Log::i("audio" , "%s info channel: %d  sampleRate: %d  outputFormat : %d" , 
            path.c_str() , 
            entity->decoder.outputChannels,
            entity->decoder.outputSampleRate,
            entity->decoder.outputFormat);


        ma_uint64 pcmLength;
        if(ma_data_source_get_length_in_pcm_frames(&entity->decoder, &pcmLength)!= MA_SUCCESS){
            Log::e("audio" , "framesRead occur error!");
        }
        Log::i("audio" , "frames length: %lu" , pcmLength);
        entity->pcmFrameLength = pcmLength;
        const double durationTime = static_cast<double>(pcmLength) / entity->decoder.outputSampleRate;
        entity->duration = durationTime;
        Log::i("audio" , "durationTime: %f" , durationTime);
        

        entity->dataCallback = [](ma_device* pDevice, void* pOutput, const void* pInput, 
            ma_uint32 frameCount){
            const AudioEntityUserData *userData = static_cast<AudioEntityUserData *>(pDevice->pUserData);
            AudioEntity *audioEntity = static_cast<AudioEntity *>(userData->entity);

            ma_decoder* pDecoder = &audioEntity->decoder;
            if (pDecoder == nullptr) {
                std::cout << "audio play callback deocoder is null" << std::endl;
                return;
            }

            // std::cout << "audio play " << audioEntity->isPlay << std::endl;
            if(audioEntity->isPlay){
                ma_uint64 readCount;
                ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, &readCount);
                audioEntity->readedFrame += readCount;
                // Log::i("audio","play audio %u / %u" , audioEntity->readedFrame , audioEntity->pcmFrameLength);

                if(audioEntity->onPlayProgressUpdate != nullptr){
                    audioEntity->onPlayProgressUpdate(audioEntity->readedFrame , audioEntity->pcmFrameLength , audioEntity->duration);
                }


                if(readCount < frameCount){
                    audioEntity->readedFrame = 0;
                    ma_decoder_seek_to_pcm_frame(pDecoder , 0);

                    if(audioEntity->onPlayEndCallback != nullptr){
                        audioEntity->onPlayEndCallback(audioEntity->name);
                    }

                    if(!(audioEntity->isLoop)){
                        audioEntity->isPlay = false;
                    }
                }
            }
        };

        entity->deviceConfig = ma_device_config_init(ma_device_type_playback);
        entity->deviceConfig.playback.format   = entity->decoder.outputFormat;
        entity->deviceConfig.playback.channels = entity->decoder.outputChannels;
        entity->deviceConfig.sampleRate        = entity->decoder.outputSampleRate;
        entity->userData.entity = entity.get();

        entity->deviceConfig.pUserData = &(entity->userData);
        entity->deviceConfig.dataCallback = entity->dataCallback;

        if(ma_device_init(nullptr, &(entity->deviceConfig), &(entity->device)) != MA_SUCCESS) {
            Log::i("audio" , "loadAudioResource ma_device_init error");
            ma_decoder_uninit(&(entity->decoder));
            return nullptr;
        }

        entity->isPlay = false;
        return entity;
    }

    void AudioManager::playAudioEntity(std::shared_ptr<AudioEntity> entity){
        if(entity == nullptr){
            return;
        }

        entity->isPlay = true;
        if (ma_device_start(&(entity->device)) != MA_SUCCESS) {
            Log::i("audio" , "Failed to start playback device");
        }
    }

    void AudioManager::stopAudioEntity(std::shared_ptr<AudioEntity> entity){
        if(entity == nullptr){
            return;
        }

        entity->isPlay = false;
        if (ma_device_stop(&(entity->device)) != MA_SUCCESS) {
            Log::i("audio" , "Failed to stop playback device");
        }
    }

    bool AudioManager::isPlayAudioEntity(std::shared_ptr<AudioEntity> entity){
        if(entity == nullptr){
            return false;
        }

        return entity->isPlay;
    }

    void AudioManager::restartAudioEntity(std::shared_ptr<AudioEntity> entity){
        if(entity == nullptr){
            return;
        }

        ma_decoder_seek_to_pcm_frame(&entity->decoder , 0);
        playAudioEntity(entity);
    }

    void AudioManager::setAudioPlayProgressCallback(std::shared_ptr<AudioEntity> entity , ProgressUpdateCallbackFn callback){
        if(entity == nullptr){
            return;
        }

        entity->setPlayProgressUpdateCallback(callback);
    }

    void AudioManager::setAudioPlayEndCallback(std::shared_ptr<AudioEntity> entity , PlayEndCallbackFn callback){
        if(entity == nullptr){
            return;
        }

        entity->setPlayEndCallback(callback);
    }

    AudioInfo AudioManager::getAudioEntityInfo(std::shared_ptr<AudioEntity> entity){
        AudioInfo retInfo;

        if(entity == nullptr){
            return retInfo;
        }

        retInfo.duration = entity->duration;
        retInfo.isLoop = entity->isLoop;
        retInfo.isPlay = entity->isPlay;
        retInfo.pcmFrameLength = entity->pcmFrameLength;
        retInfo.readedFrame = entity->readedFrame;
        retInfo.name = entity->name;

        return retInfo;
    }

    void AudioManager::releaseAudioEntity(std::shared_ptr<AudioEntity> entity){
        if(entity == nullptr){
            return;
        }

        Log::i("audio" , "audio %s release" , entity->name.c_str());

        if(entity->isReleased){
            Log::i("audio" , "this audio %s had been released" , entity->name.c_str());
            return;    
        }

        entity->isPlay = false;
        ma_device_stop(&(entity->device));

        ma_device_uninit(&(entity->device));
        ma_decoder_uninit(&(entity->decoder));
        entity->pData = nullptr;
        entity->isReleased = true;
    }

    //载入音频资源
    std::shared_ptr<AudioEntity> AudioManager::loadAudio(std::string path,
        std::string key , bool playloop){
        auto entity = loadAudioEntity(path , playloop);
        if(audioMap_[key] != nullptr){
            releaseAudioEntity(audioMap_[key]);
        }

        audioMap_[key] = entity;
        return entity;
    }

    //播放音频
    void AudioManager::playAudio(std::string key){
        auto entity = audioMap_[key];
        playAudioEntity(entity);
    }

    //停止播放
    void AudioManager::stopAudio(std::string key){
        auto entity = audioMap_[key];
        stopAudioEntity(entity);
    }

    //重新开始播放
    void AudioManager::restartAudio(std::string key){
        auto entity = audioMap_[key];
        restartAudioEntity(entity);
    }

    //
    void AudioManager::releaseAudio(std::string key){
        auto entity = audioMap_[key];
        releaseAudioEntity(entity);
        audioMap_.erase(key);
    }

    void AudioManager::init(){
        Log::i("audio" , "audio manager init");
    }

    //释放载入的音频资源    
    void AudioManager::dispose(){
        Log::i("audio" , "audio manager dispose");

        for(auto &pair : audioMap_){
            if(pair.second != nullptr){
                releaseAudioEntity(pair.second);
            }
        }//end for each;

        audioMap_.clear();
    }

}

