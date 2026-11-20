#include "hook.h"
#include "overlay_gl.h"
#include <android/log.h>
#include <EGL/egl.h>
#include <dlfcn.h>
#include <jni.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Gepstorm",__VA_ARGS__)
extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM*, void*){ LOGI("JNI_OnLoad"); return JNI_VERSION_1_6; }
extern "C" JNIEXPORT void JNICALL
Java_com_gepstorm_manager_MainActivity_nativeHook(JNIEnv* env, jobject thiz){
    extern void set_auth_context(JNIEnv*,jobject);
    set_auth_context(env,thiz);
    void* s=dlsym(RTLD_DEFAULT,"eglSwapBuffers");
    if(s) hook_install((FnEglSwap)s,hook_eglSwapBuffers);
    else LOGI("eglSwapBuffers not found");
}
