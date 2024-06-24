#include "lib.hpp"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "fs.h" 
#include "imgui_nvn.h" 
#include <nya.h> 

HOOK_DEFINE_TRAMPOLINE(Call_nngetalloc) { 
    static nn::mem::StandardAllocator* Callback() { 
        nya::Allocator = Orig(); 
        return nya::Allocator; 
    }
}; 
 
HOOK_DEFINE_TRAMPOLINE(nnMain_hook) { 
    static void Callback() { 
        nn::fs::MountSdCardForDebug("sd"); 
        Orig(); 
    }
}; 

extern "C" void exl_main(void *x0, void *x1) { 
    exl::hook::Initialize(); 
    Call_nngetalloc::InstallAtOffset(0x009ab090); 
    nnMain_hook::InstallAtSymbol("nnMain"); 
    nvnImGui::InstallHooks();
    nvnImGui::addDrawFunc(nya::nya_main);
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}