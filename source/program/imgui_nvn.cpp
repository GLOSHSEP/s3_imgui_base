#include <oe.h>
#include "imgui_nvn.h"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "init.h"
#include "lib.hpp"
#include "nvn_CppFuncPtrImpl.h"
#include "utilites/input_mgr.h" 

nvn::Device *nvnDevice;
nvn::Queue *nvnQueue;
nvn::CommandBuffer *nvnCmdBuf;

nvn::DeviceGetProcAddressFunc tempGetProcAddressFuncPtr;

nvn::CommandBufferInitializeFunc tempBufferInitFuncPtr;
nvn::DeviceInitializeFunc tempDeviceInitFuncPtr;
nvn::QueueInitializeFunc tempQueueInitFuncPtr;
nvn::QueuePresentTextureFunc tempPresentTexFunc;

nvn::WindowSetCropFunc tempSetCropFunc;

bool hasInitImGui = false; 

namespace nvnImGui {
    ImVector<ProcDrawFunc> drawQueue;
}

#define IMGUI_USEEXAMPLE_DRAW false

void setCrop(nvn::Window *window, int x, int y, int w, int h) {
    tempSetCropFunc(window, x, y, w, h);

    if (hasInitImGui) {

        ImVec2 &dispSize = ImGui::GetIO().DisplaySize;
        ImVec2 windowSize = ImVec2(w - x, h - y);

        if (dispSize.x != windowSize.x && dispSize.y != windowSize.y) {
            bool isDockedMode = nn::oe::GetOperationMode() == nn::oe::OperationMode_Docked;

            dispSize = windowSize;
            ImguiNvnBackend::updateProjection(windowSize);
            ImguiNvnBackend::updateScale(isDockedMode);

        }
    }
}

void presentTexture(nvn::Queue *queue, nvn::Window *window, int texIndex) {

    if (hasInitImGui)
        nvnImGui::procDraw();

    tempPresentTexFunc(queue, window, texIndex);
}

NVNboolean deviceInit(nvn::Device *device, const nvn::DeviceBuilder *builder) {
    NVNboolean result = tempDeviceInitFuncPtr(device, builder);
    nvnDevice = device;
    nvn::nvnLoadCPPProcs(nvnDevice, tempGetProcAddressFuncPtr);
    return result;
}

NVNboolean queueInit(nvn::Queue *queue, const nvn::QueueBuilder *builder) {
    NVNboolean result = tempQueueInitFuncPtr(queue, builder);
    nvnQueue = queue;
    return result;
}

NVNboolean cmdBufInit(nvn::CommandBuffer *buffer, nvn::Device *device) {
    NVNboolean result = tempBufferInitFuncPtr(buffer, device);
    nvnCmdBuf = buffer;

    if (!hasInitImGui) {
        hasInitImGui = nvnImGui::InitImGui();
    }

    return result;
}

nvn::GenericFuncPtrFunc getProc(nvn::Device *device, const char *procName) {

    nvn::GenericFuncPtrFunc ptr = tempGetProcAddressFuncPtr(nvnDevice, procName);

    if (strcmp(procName, "nvnQueueInitialize") == 0) {
        tempQueueInitFuncPtr = (nvn::QueueInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &queueInit;
    } else if (strcmp(procName, "nvnCommandBufferInitialize") == 0) {
        tempBufferInitFuncPtr = (nvn::CommandBufferInitializeFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &cmdBufInit;
    } else if (strcmp(procName, "nvnWindowSetCrop") == 0) {
        tempSetCropFunc = (nvn::WindowSetCropFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &setCrop;
    } else if (strcmp(procName, "nvnQueuePresentTexture") == 0) {
        tempPresentTexFunc = (nvn::QueuePresentTextureFunc) ptr;
        return (nvn::GenericFuncPtrFunc) &presentTexture;
    }

    return ptr;
} 

HOOK_DEFINE_TRAMPOLINE(NvnBootstrapHook) {
    static void *Callback(const char *funcName) {

        void *result = Orig(funcName);

        if (strcmp(funcName, "nvnDeviceInitialize") == 0) {
            tempDeviceInitFuncPtr = (nvn::DeviceInitializeFunc) result;
            return (void *) &deviceInit;
        }
        if (strcmp(funcName, "nvnDeviceGetProcAddress") == 0) {
            tempGetProcAddressFuncPtr = (nvn::DeviceGetProcAddressFunc) result;
            return (void *) &getProc;
        }

        return result;
    }
}; 

void nvnImGui::addDrawFunc(ProcDrawFunc func) {

    EXL_ASSERT(!drawQueue.contains(func), "Function has already bneen added to queue!");

    drawQueue.push_back(func);
}

void nvnImGui::procDraw() {

    ImguiNvnBackend::newFrame();
    ImGui::NewFrame();

    for (auto drawFunc: drawQueue) {
        drawFunc();
    }

    ImGui::Render();
    ImguiNvnBackend::renderDrawData(ImGui::GetDrawData());
}

void nvnImGui::InstallHooks() {
    NvnBootstrapHook::InstallAtSymbol("nvnBootstrapLoader");
}

bool nvnImGui::InitImGui() {
    if (nvnDevice && nvnQueue && nvnCmdBuf) { 

        IMGUI_CHECKVERSION();

        ImGuiMemAllocFunc allocFunc = [](size_t size, void* user_data) {
            return aligned_alloc(0x1000, size);
        };

        ImGuiMemFreeFunc freeFunc = [](void* ptr, void* user_data) { free(ptr); };

        ImGui::SetAllocatorFunctions(allocFunc, freeFunc, nullptr);

        ImGui::CreateContext(); 

        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        ImGui::StyleColorsDark();

        ImguiNvnBackend::NvnBackendInitInfo initInfo = {
                .device = nvnDevice,
                .queue = nvnQueue,
                .cmdBuf = nvnCmdBuf
        };

        ImguiNvnBackend::InitBackend(initInfo);

        nya::hid::initKBM();

        nya::hid::setPort(0);

        return true;
    } else { 
        return false;
    }
}