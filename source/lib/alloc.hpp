#ifndef CLANGD_SUCKS
#pragma once 

#include <cstddef> 

extern "C" { 
    //main heap 
    void* malloc(size_t size); 
    void free(void* ptr); 
    void* aligned_alloc(size_t alignment, size_t size); 
}; 

//main heap 
void* operator new(size_t size); 
void operator delete(void* ptr) noexcept;
#endif
