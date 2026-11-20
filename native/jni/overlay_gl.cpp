#include "overlay_gl.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "esp.h"
#include "aim.h"
#include "bhop.h"
#include "snow.h"
#include "config.h"
#include "il2cpp_resolver.h"
#include "offsets.h"
#include "auth_bridge.h"
#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <cstring>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"Gepstorm",__VA_ARGS__)
static Il2Cpp g_il2; static Config g_cfg; static Snow g_snow;
static int g_w=0,g_h=0; static float g_vm[16]{};
static Il2CppClass* cPlayer=nullptr; static Il2CppClass* cWorld=nullptr;
static size_t oPos=0,oHead=0,oTeam=0,oHp=0,oAlive=0,oVel=0,oGnd=0,oVA=0,oList=0,oLocal=0,oVM=0;
static void* g_world=nullptr; static void* g_local=nullptr;
static bool g_inited=false, g_auth_ok=false;
static JNIEnv* g_env=nullptr; static jobject g_ctx=nullptr;
extern "C" void set_auth_context(JNIEnv* e,jobject c){ g_env=e; g_ctx=(e&&c)?e->NewGlobalRef(c):nullptr; }
void overlay_init(int w,int h){
    g_w=w; g_h=h;
    if(!g_il2.init()){ LOGI("il2cpp init failed"); return; }
    cPlayer=g_il2.find_class(off::NS_GAME,off::CLS_PLAYER);
    cWorld =g_il2.find_class(off::NS_GAME,off::CLS_WORLD);
    if(cPlayer){
        oPos=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_POSITION));
        oHead=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_HEAD));
        oTeam=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_TEAM));
        oHp=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_HEALTH));
        oAlive=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_ALIVE));
        oVel=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_VELOCITY));
        oGnd=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_GROUNDED));
        oVA=g_il2.field_offset(g_il2.find_field(cPlayer,off::F_VIEWANGL));
    }
    if(cWorld){
        oList=g_il2.field_offset(g_il2.find_field(cWorld,off::F_PLAYER_LIST));
        oLocal=g_il2.field_offset(g_il2.find_field(cWorld,off::F_LOCAL_PLAYER));
        oVM=g_il2.field_offset(g_il2.find_field(cWorld,off::F_VIEW_MATRIX));
    }
    g_cfg=Config::load("/sdcard/Gepstorm/config/gepstorm.json");
    g_snow.init(w,h);
    ImGui::CreateContext(); ImGui::GetIO().IniFilename=nullptr;
    ImGui_ImplOpenGL3_Init("#version 300 es");
}
void on_frame(){
    if(!g_inited){
        EGLDisplay d=eglGetCurrentDisplay(); EGLSurface s=eglGetCurrentSurface(EGL_DRAW);
        EGLint w=0,h=0; eglQuerySurface(d,s,EGL_WIDTH,&w); eglQuerySurface(d,s,EGL_HEIGHT,&h);
        overlay_init(w,h); g_inited=true;
    }
    if(!g_auth_ok){
        if(g_env&&g_ctx){ auto t=auth_read_token(g_env,g_ctx); g_auth_ok=auth_verify_token(t,"jwt","",""); }
        if(!g_auth_ok) return;
    }
    ImGui_ImplOpenGL3_NewFrame(); ImGui::NewFrame();
    ImDrawList* dl=ImGui::GetBackgroundDrawList();
    PlayerSnapshot list[64]; int n=0; PlayerSnapshot local{};
    if(g_world&&g_local){
        auto* vm=reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(g_world)+oVM);
        if(vm) memcpy(g_vm,vm,sizeof(g_vm));
        local.pos=g_il2.read<Vec3>(g_local,oPos);
        local.head=g_il2.read<Vec3>(g_local,oHead);
        local.team=g_il2.read<int>(g_local,oTeam);
        local.hp=g_il2.read<int>(g_local,oHp);
        local.alive=g_il2.read<bool>(g_local,oAlive);
        void* pl=*reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(g_world)+oList);
        if(pl){
            void* arr=*reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(pl)+0x10);
            int sz=*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(pl)+0x18);
            if(sz>64) sz=64;
            for(int i=0;i<sz;++i){
                void* p=*reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(arr)+0x20+i*8);
                if(!p) continue;
                PlayerSnapshot s;
                s.pos=g_il2.read<Vec3>(p,oPos); s.head=g_il2.read<Vec3>(p,oHead);
                s.team=g_il2.read<int>(p,oTeam); s.hp=g_il2.read<int>(p,oHp);
                s.alive=g_il2.read<bool>(p,oAlive);
                if(s.alive) list[n++]=s;
            }
        }
    }
    if(g_cfg.esp_enabled&&n>0)
        esp_render(dl,list,n,local,g_vm,(float)g_w,(float)g_h,
                   g_cfg.esp_box,g_cfg.esp_name,g_cfg.esp_health,g_cfg.esp_skel);
    if(g_cfg.aim_enabled&&g_local&&n>0&&ImGui::IsMouseDown(ImGuiMouseButton_Right)){
        Vec3 t;
        if(aim_best(list,n,local,g_vm,(float)g_w,(float)g_h,
                    g_cfg.aim_fov,g_cfg.aim_range,g_cfg.aim_bone,t))
            aim_apply(g_il2,g_local,oVA,t,g_cfg.aim_smooth);
    }
    if(g_cfg.bhop_enabled&&g_local)
        bhop_tick(g_il2,g_local,oVel,oGnd,ImGui::IsMouseDown(ImGuiMouseButton_Left));
    if(g_cfg.snow_enabled){ g_snow.update(ImGui::GetIO().DeltaTime,g_w,g_h); g_snow.render(dl,g_w,g_h); }
    ImGui::SetNextWindowPos({20,20},ImGuiCond_FirstUseEver);
    ImGui::Begin("Gepstorm v1.1");
    ImGui::Checkbox("Aim",&g_cfg.aim_enabled);
    ImGui::SliderFloat("FOV",&g_cfg.aim_fov,10,500);
    ImGui::SliderFloat("Smooth",&g_cfg.aim_smooth,1,20);
    ImGui::Checkbox("Bhop",&g_cfg.bhop_enabled);
    ImGui::Checkbox("ESP",&g_cfg.esp_enabled);
    ImGui::Checkbox("WH",&g_cfg.wh_enabled);
    ImGui::Checkbox("Snow",&g_cfg.snow_enabled);
    if(ImGui::Button("Save")) g_cfg.save("/sdcard/Gepstorm/config/gepstorm.json");
    ImGui::End();
    ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
