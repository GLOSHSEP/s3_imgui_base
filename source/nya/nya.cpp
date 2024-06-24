#include "nya/nya.h" 
#include "program/imgui_nvn.h"
#include "nya/windows/intro.h" 
#include "nya/windows/bar.h" 

#include <nn/init.h> 

namespace nya { 
    //menu 
    bool show_menu = false; 

    //windows 
    bool show_intro = true; 

    //memory 
    nn::mem::StandardAllocator* Allocator = nullptr; 

    //main loop 
    void nya_main() { 
        //windows 
        window::bar();
        window::intro(); 
    } 
}