#pragma once 

#include <nn/init.h> 

namespace nya { 
    //menu 
    extern bool show_menu; 

    //windows 
    extern bool show_intro; 

    //memory 
    extern nn::mem::StandardAllocator* Allocator; 

    //main loop 
    void nya_main(); 
}