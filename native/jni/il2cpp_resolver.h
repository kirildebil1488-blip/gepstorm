#pragma once
#include <cstdint>
#include <string>
struct Il2CppClass; struct Il2CppDomain; struct Il2CppAssembly; struct Il2CppImage; struct FieldInfo;
class Il2Cpp {
public:
    bool init();
    Il2CppClass* find_class(const char* ns,const char* name,const char* img="Assembly-CSharp.dll");
    FieldInfo* find_field(Il2CppClass*, const char*);
    size_t field_offset(FieldInfo*);
    template<typename T> T read(void* o,size_t off){ if(!o) return T{}; return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(o)+off); }
    template<typename T> void write(void* o,size_t off,const T& v){ if(!o) return; *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(o)+off)=v; }
private:
    Il2CppDomain* (*dg_)()=nullptr;
    Il2CppAssembly* (*da_)(Il2CppDomain*,const char*)=nullptr;
    Il2CppImage* (*ai_)(Il2CppAssembly*)=nullptr;
    Il2CppClass* (*cf_)(Il2CppImage*,const char*,const char*)=nullptr;
    FieldInfo* (*ff_)(Il2CppClass*,const char*)=nullptr;
    size_t (*fo_)(FieldInfo*)=nullptr;
};
