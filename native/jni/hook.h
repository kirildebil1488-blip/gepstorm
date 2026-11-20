#pragma once
#include <EGL/egl.h>
using FnEglSwap = EGLBoolean(*)(EGLDisplay, EGLSurface);
void hook_install(FnEglSwap, FnEglSwap);
EGLBoolean hook_eglSwapBuffers(EGLDisplay, EGLSurface);
