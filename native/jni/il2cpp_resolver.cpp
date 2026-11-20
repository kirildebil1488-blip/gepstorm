#include "il2cpp_resolver.h"
#include "offsets.h"
#include <dlfcn.h>
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Gepstorm",__VA_ARGS__)
bool Il2Cpp::init(){
    void* h=dlopen(off::GAME_LIB.c_str(),RTLD_LAZY|RTLD_NOLOAD);
    if(!h) h=dlopen(off::GAME_LIB.c_str(),RTLD_LAZY);
    if(!h){ LOGI("dlopen %s fail",off::GAME_LIB.c_str()); return false; }
    dg_=(decltype(dg_))dlsym(h,off::IL2CPP_DOMAIN_GET);
    da_=(decltype(da_))dlsym(h,off::IL2CPP_DOMAIN_ASSEMBLY);
    ai_=(decltype(ai_))dlsym(h,off::IL2CPP_ASM_IMAGE);
    cf_=(decltype(cf_))dlsym(h,off::IL2CPP_CLASS_FROM_NAME);
    ff_=(decltype(ff_))dlsym(h,off::IL2CPP_FIELD_FROM_NAME);
    fo_=(decltype(fo_))dlsym(h,off::IL2CPP_FIELD_OFFSET);
    if(!dg_||!da_||!ai_||!cf_||!ff_||!fo_){ LOGI("missing il2cpp sym"); return false; }
    return true;
}
Il2CppClass* Il2Cpp::find_class(const char* ns,const char* name,const char* img){
    auto d=dg_(); auto a=da_(d,img); if(!a) return nullptr;
    return cf_(ai_(a),ns,name);
}
FieldInfo* Il2Cpp::find_field(Il2CppClass* k,const char* n){ return k?ff_(k,n):nullptr; }
size_t Il2Cpp::field_offset(FieldInfo* f){ return f?fo_(f):0; }
