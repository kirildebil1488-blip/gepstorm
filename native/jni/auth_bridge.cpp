#include "auth_bridge.h"
std::string auth_read_token(JNIEnv* env, jobject context){
    if(!context) return {};
    jclass cls=env->GetObjectClass(context);
    jmethodID mid=env->GetMethodID(cls,"getSharedPreferences","(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
    if(!mid) return {};
    jobject prefs=env->CallObjectMethod(context,mid,env->NewStringUTF("gepstorm"),0);
    jclass pcls=env->GetObjectClass(prefs);
    jmethodID get=env->GetMethodID(pcls,"getString","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    jstring val=(jstring)env->CallObjectMethod(prefs,get,env->NewStringUTF("auth_token"),env->NewStringUTF(""));
    std::string out; if(val){ const char* c=env->GetStringUTFChars(val,nullptr); out=c; env->ReleaseStringUTFChars(val,c);}
    return out;
}
bool auth_verify_token(const std::string& t,const std::string& mode,const std::string&,const std::string&){
    if(t.empty()) return false;
    if(mode=="opaque") return t.size()>=32;
    if(mode=="static") return t.rfind("GEP-",0)==0;
    int d=0; for(char c:t) if(c=='.') ++d; return d==2;
}
