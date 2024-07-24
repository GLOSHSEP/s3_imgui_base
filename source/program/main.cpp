#include "lib.hpp"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "fs.h" 
#include "imgui_nvn.h" 
#include <nya.h> 
 
HOOK_DEFINE_TRAMPOLINE(nnMain_hook) { 
    static void Callback() { 
        nn::fs::MountSdCardForDebug("sd"); 
        Orig(); 
    }
}; 

extern "C" void exl_main(void *x0, void *x1) { 
    exl::hook::Initialize(); 
    nnMain_hook::InstallAtSymbol("nnMain"); 
    nvnImGui::InstallHooks();
    nvnImGui::addDrawFunc(nya::nya_main);
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}