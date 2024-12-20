#pragma once

#include "glheader.h"
#include <memory>
#include <vector>
#include "log.h"

namespace purple{
    struct MemoryAllocatorInfo{
        unsigned int bufferId = 0;
        unsigned int vao = 0;
        int size = 0;
        int offset = 0;
    };

    //显存分配器
    class VRamAllcator{
    private:
        std::vector<std::shared_ptr<MemoryAllocatorInfo>> allocatedList_; //已分配的显存记录表

        unsigned int currentBufferIdIndex_ = 0;
        
        unsigned int genBuffer();

        void createNewBuffer();
    public:
        VRamAllcator(){}

        const int ALLOCATOR_SIZE = 4 * 1024 * 1024;//一次分配4M缓存

        int fetchVideoMemory(int requestSize ,
                unsigned int &bufferId , unsigned int &vao,
                int &offset , int &size);

        void recycleAllMemory();

        void freeMemory();
    };

    //显存管理
    class VRamManager{
    public:
        const char *TAG = "VRamManager";
        
        // static std::shared_ptr<VRamManager> getInstance();
        
        // static std::shared_ptr<VRamManager> instance_;
        
        void onPostRender();

        void clear();

        //获取一块显存 用于写入内容
        int fetchVideoMemory(int requestSize , 
                unsigned int &bufferId , 
                unsigned int &vao,
                int &offset , int &size);
        
        ~VRamManager(){
            Log::i(TAG , "~VRamManager deconstruct");
        }
        
        VRamManager();
    private:
        std::shared_ptr<VRamAllcator> allocator_ = nullptr;

    };
}




