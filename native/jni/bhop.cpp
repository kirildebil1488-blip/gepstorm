#include "bhop.h"
#include "math.h"
void bhop_tick(Il2Cpp& il,void* lp,size_t offVel,size_t offG,bool space){
    if(!lp||!space) return;
    if(!il.read<bool>(lp,offG)) return;
    Vec3 v=il.read<Vec3>(lp,offVel);
    float hs=std::sqrt(v.x*v.x+v.y*v.y);
    if(hs>1.f){ float k=320.f/hs; v.x*=k; v.y*=k; }
    v.z=320.f; il.write<Vec3>(lp,offVel,v);
}
