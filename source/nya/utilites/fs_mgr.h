#pragma once

#include "nn/result.h" 

namespace nya { 
    namespace fs { 

        struct LoadData {
            const char* path;
            void* buffer;
            long bufSize;
        };

        nn::Result writeFileToPath(void* buf, size_t size, const char* path);

        void loadFileFromPath(LoadData& loadData, size_t alignment);

        long getFileSize(const char* path);

        bool isFileExist(const char* path); 
    } 
}
