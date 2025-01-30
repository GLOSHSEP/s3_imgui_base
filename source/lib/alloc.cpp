#ifndef CLANGD_SUCKS
#include <alloc.hpp> 
#include <nn/lmem.h>
#include <nya.h>

extern "C" { 
    //main heap 
    void* malloc(size_t size) { 
        void* allocated = nn::lmem::AllocateFromExpHeap(nya::MainHeap, size, 8); 
        //nya::nya_log("[allocation] malloc size of %zu\n[allocation] malloc returned pointer %p\n", size, allocated);  
        return allocated; 
    } 
    void free(void* ptr) { 
        //nya::nya_log("[allocation] free of pointer %p\n", ptr); 
        nn::lmem::FreeToExpHeap(nya::MainHeap, ptr); 
    } 
    void* aligned_alloc(size_t alignment, size_t size) { 
        void* allocated = nn::lmem::AllocateFromExpHeap(nya::MainHeap, size, alignment); 
        //nya::nya_log("[allocation] aligned_alloc size of %zu aligment of %zu\n[allocation] aligned_alloc returned pointer %p\n", size, alignment, allocated);  
        return allocated; 
    }
}; 

//main heap 
void* operator new(size_t size) { /*nya::nya_log("[allocation] operator new call\n");*/ return malloc(size); }
void operator delete(void* ptr) { /*nya::nya_log("[allocation] operator delete call\n");*/ free(ptr); } 
#endif 