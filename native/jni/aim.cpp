#include "aim.h"
#include "esp.h"
#include "il2cpp_resolver.h"
#include <limits>
bool aim_best(const PlayerSnapshot* ps,int n,const PlayerSnapshot& l,
              const float vm[16],float W,float H,float fov,float range,int bone,Vec3& out){
    Vec2 c{W*0.5f,H*0.5f}; float best=std::numeric_limits<float>::max(); bool ok=false;
    for(int i=0;i<n;++i){ const auto& p=ps[i]; if(!p.alive||p.team==l.team) continue;
        Vec3 t=(bone==0)?p.head:Vec3{p.pos.x,p.pos.y,p.pos.z+48.f};
        if(len(t-l.head)>range) continue;
        Vec2 s; if(!w2s(vm,t,W,H,s)) continue;
        float dx=s.x-c.x,dy=s.y-c.y,d=std::sqrt(dx*dx+dy*dy);
        if(d>fov||d>=best) continue; best=d; out=calc_angle(l.head,t); ok=true;
    } return ok;
}
void aim_apply(Il2Cpp& il,void* lp,size_t off,Vec3 t,float sm){
    if(!lp) return; Vec3 cur=il.read<Vec3>(lp,off);
    while(t.y-cur.y> 180.f) t.y-=360.f; while(t.y-cur.y<-180.f) t.y+=360.f;
    float s=std::max(1.f,sm);
    il.write<Vec3>(lp,off,Vec3{cur.x+(t.x-cur.x)/s,cur.y+(t.y-cur.y)/s,0.f});
}
