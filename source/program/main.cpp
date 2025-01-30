#include "lib/hook/trampoline.hpp"
#include "nn/fs.h" 
#include "imgui_nvn.h" 
#include "nya.h"
 
HOOK_DEFINE_TRAMPOLINE(nnMain_hook) { 
    static void Callback() { 
        nn::fs::MountSdCardForDebug("sd");
        nya::nya_sd_avalible = true; 
        nya::nya_log("[nya] hewwo wowld :3\n");
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