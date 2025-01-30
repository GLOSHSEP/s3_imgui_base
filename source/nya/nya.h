#pragma once 

#include "nn/lmem.h"

namespace nya {
    //heap
    using HeapHandle = nn::lmem::detail::HeapHead*;
    extern HeapHandle MainHeap; 

    //windows 
    extern bool show_window;

    //misc 
    extern bool nya_sd_avalible;

    //log stuff
    void nya_log(const char* message, ...);

    //main loop 
    void nya_main(); 
}
