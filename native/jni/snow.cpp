#include "snow.h"
#include <random>
#include <cmath>
#include <algorithm>
static std::mt19937 rng{std::random_device{}()};
static float rnd(float a,float b){return std::uniform_real_distribution<float>(a,b)(rng);}
void Snow::init(int w,int h,int n){ n_=std::min(n,MAX);
    for(int i=0;i<n_;++i) f_[i]={rnd(0,(float)w),rnd(-h,(float)h),rnd(-15,15),rnd(40,90),rnd(1,3),rnd(0,6.28f)}; }
void Snow::update(float dt,int w,int h){
    for(int i=0;i<n_;++i){ auto& f=f_[i]; f.ph+=dt*2.f;
        f.x+=(f.vx+std::sin(f.ph)*20.f)*dt; f.y+=f.vy*dt;
        if(f.y>h+4){f.y=-4;f.x=rnd(0,(float)w);} if(f.x<-4)f.x=w+4; if(f.x>w+4)f.x=-4; } }
void Snow::render(ImDrawList* dl,int w,int h){
    for(int i=0;i<n_;++i) dl->AddCircleFilled({f_[i].x,f_[i].y},f_[i].r,IM_COL32(240,245,255,180));
    dl->AddTriangleFilled({w-120.f,80.f},{w-60.f,80.f},{w-90.f,10.f},IM_COL32(20,140,60,220));
    dl->AddTriangleFilled({w-130.f,120.f},{w-50.f,120.f},{w-90.f,40.f},IM_COL32(30,160,70,220));
    dl->AddRectFilled({w-96.f,120.f},{w-84.f,150.f},IM_COL32(120,80,40,255));
    dl->AddCircleFilled({w-90.f,8.f},6.f,IM_COL32(255,220,80,255));
}
