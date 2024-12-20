#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>

namespace purple{
    struct AudioEntity;

    struct AudioInfo{
        std::string name = "";
        bool isPlay = false;
        bool isLoop = false;
        unsigned long pcmFrameLength = 0;
        float duration = 0.0f;
        unsigned long readedFrame = 0;
    };

    typedef std::function<void(unsigned long, unsigned long , double)> ProgressUpdateCallbackFn;

    typedef std::function<void(std::string path)> PlayEndCallbackFn;

    class AudioManager{
    public:
        static std::shared_ptr<AudioManager> getInstance();

        void init();

        //载入音频资源
        std::shared_ptr<AudioEntity> loadAudio(std::string path , std::string key ,bool playloop = false);

        //播放音频
        void playAudio(std::string key);

        //停止播放
        void stopAudio(std::string key);

        //重新开始播放
        void restartAudio(std::string key);

        //释放资源
        void releaseAudio(std::string key);
        
        void dispose();

        std::shared_ptr<AudioEntity> loadAudioEntity(std::string path , bool playLoop = false);

        void releaseAudioEntity(std::shared_ptr<AudioEntity> entity);

        void playAudioEntity(std::shared_ptr<AudioEntity> entity);

        void stopAudioEntity(std::shared_ptr<AudioEntity> entity);

        bool isPlayAudioEntity(std::shared_ptr<AudioEntity> entity);

        void restartAudioEntity(std::shared_ptr<AudioEntity> entity);

        void setAudioPlayProgressCallback(std::shared_ptr<AudioEntity> entity , ProgressUpdateCallbackFn callback);

        AudioInfo getAudioEntityInfo(std::shared_ptr<AudioEntity> entity);

        void setAudioPlayEndCallback(std::shared_ptr<AudioEntity> entity , PlayEndCallbackFn callback);
    private:
        std::map<std::string , std::shared_ptr<AudioEntity>> audioMap_;
    };
}




