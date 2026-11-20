#include "hook.h"
#include <sys/mman.h>
#include <cstring>
#include <cstdint>
#include <unistd.h>

// минимальный inline-хук под arm64 без внешних библиотек.
// на входе: original — адрес функции, replacement — наша функция.
// сохраняем 16 байт оригинала, пишем:
//   LDR x16, #8   (0x58000050)
//   BR  x16       (0xD61F0200)
//   .quad replacement
// trampoline: копия 16 байт оригинала + LDR x16,#8; BR x16; .quad original+16

static uint8_t* g_trampoline = nullptr;
static FnEglSwap g_orig = nullptr;

static void make_rwx(void* p, size_t n){
    uintptr_t page = reinterpret_cast<uintptr_t>(p) & ~(uintptr_t)(getpagesize()-1);
    mprotect(reinterpret_cast<void*>(page), n + (reinterpret_cast<uintptr_t>(p)-page), PROT_READ|PROT_WRITE|PROT_EXEC);
}

void hook_install(FnEglSwap original, FnEglSwap replacement){
    g_orig = original;
    if (!g_trampoline){
        g_trampoline = static_cast<uint8_t*>(mmap(nullptr, 4096, PROT_READ|PROT_WRITE|PROT_EXEC,
                                                  MAP_PRIVATE|MAP_ANONYMOUS, -1, 0));
    }
    // trampoline: 16 байт оригинала + прыжок назад на original+16
    memcpy(g_trampoline, original, 16);
    uint32_t* t = reinterpret_cast<uint32_t*>(g_trampoline + 16);
    t[0] = 0x58000050; // LDR x16, #8
    t[1] = 0xD61F0200; // BR  x16
    *reinterpret_cast<uint64_t*>(g_trampoline + 24) =
        reinterpret_cast<uint64_t>(reinterpret_cast<uint8_t*>(original) + 16);

    // патчим оригинал
    make_rwx(reinterpret_cast<void*>(original), 32);
    uint32_t* o = reinterpret_cast<uint32_t*>(original);
    o[0] = 0x58000050; // LDR x16, #8
    o[1] = 0xD61F0200; // BR  x16
    *reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(original) + 8) =
        reinterpret_cast<uint64_t>(replacement);

    __builtin___clear_cache(reinterpret_cast<char*>(original),
                            reinterpret_cast<char*>(original) + 32);
}

EGLBoolean hook_eglSwapBuffers(EGLDisplay d, EGLSurface s){
    extern void on_frame();
    on_frame();
    using TrampFn = EGLBoolean(*)(EGLDisplay, EGLSurface);
    auto fn = reinterpret_cast<TrampFn>(g_trampoline);
    return fn(d, s);
}
