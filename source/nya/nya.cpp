#include "nya/nya.h"
#include "nya/windows/intro.h"
#include "nn/util.h"
#include "utilites/fs_mgr.h"
#include "nn/fs.h"
#include "nn/fs/fs_files.hpp" 

namespace nya {
    //heap
    HeapHandle MainHeap = nullptr; 

    //windows 
    bool show_window = true;

    //misc
    bool nya_sd_avalible = false;

    //log stuff
    void nya_log(const char* message, ...) {
        va_list args;
        va_start(args, message);
        nn::fs::FileHandle handle;
        static long position = 0;
        long add_size;
        static bool initialized = false;
        char buffer[128];

        if (!initialized) {
            if (!nya_sd_avalible)
                return;
            if (nya::fs::isFileExist("sd:/nyalog.txt"))
                nn::fs::DeleteFile("sd:/nyalog.txt");
            initialized = true;
        }

        if (!nya::fs::isFileExist("sd:/nyalog.txt")) {
            nn::fs::CreateFile("sd:/nyalog.txt", 0);
            position = 0;
        }

        int writen = nn::util::VSNPrintf(buffer, 128, message, args);
        va_end(args);

        nn::fs::OpenFile(&handle, "sd:/nyalog.txt", nn::fs::OpenMode_Write);
        nn::fs::GetFileSize(&add_size, handle);
        nn::fs::SetFileSize(handle, add_size + writen);
        nn::fs::WriteFile(
            handle, position, buffer, writen,
            nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));
        nn::fs::CloseFile(handle);
        position += long(writen);
    }

    //main loop 
    void nya_main() {
        //windows 
        window::demo_window();
    } 
}
