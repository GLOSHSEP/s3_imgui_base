#include "nya/nya.h" 
#include "program/imgui_nvn.h"
#include "nya/windows/intro.h" 

#include <nn/init.h> 

namespace nya { 
    //windows 
    bool show_window = true; 

    //memory 
    nn::mem::StandardAllocator* Allocator = nullptr; 

    //main loop 
    void nya_main() { 
        //windows 
        window::demo_window(); 
    } 
}
