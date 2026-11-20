#include "hook.h"
#include "dobby.h"
static FnEglSwap g_orig=nullptr;
void hook_install(FnEglSwap o,FnEglSwap r){ g_orig=o; DobbyHook((void*)o,(void*)r,(void**)&g_orig); }
EGLBoolean hook_eglSwapBuffers(EGLDisplay d,EGLSurface s){ extern void on_frame(); on_frame(); return g_orig(d,s); }
