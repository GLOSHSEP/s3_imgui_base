#pragma once 

#include <nn/init.h> 

namespace nya { 
    //windows 
    extern bool show_window; 

    //memory 
    extern nn::mem::StandardAllocator* Allocator; 

    //main loop 
    void nya_main(); 
}
