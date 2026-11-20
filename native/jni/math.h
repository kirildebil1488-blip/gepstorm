#pragma once
#include <cmath>
struct Vec2{float x,y;}; struct Vec3{float x,y,z;};
inline Vec3 operator-(Vec3 a,Vec3 b){return{a.x-b.x,a.y-b.y,a.z-b.z};}
inline Vec3 operator+(Vec3 a,Vec3 b){return{a.x+b.x,a.y+b.y,a.z+b.z};}
inline Vec3 operator*(Vec3 a,float s){return{a.x*s,a.y*s,a.z*s};}
inline float dot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
inline float len(Vec3 a){return std::sqrt(dot(a,a));}
inline bool w2s(const float m[16],Vec3 w,float W,float H,Vec2& o){
    float cx=m[0]*w.x+m[4]*w.y+m[8]*w.z+m[12];
    float cy=m[1]*w.x+m[5]*w.y+m[9]*w.z+m[13];
    float cw=m[3]*w.x+m[7]*w.y+m[11]*w.z+m[15];
    if(cw<0.01f) return false;
    o.x=(W*0.5f)+(W*0.5f)*(cx/cw);
    o.y=(H*0.5f)-(H*0.5f)*(cy/cw);
    return true;
}
inline Vec3 calc_angle(Vec3 s,Vec3 d){
    Vec3 v=d-s; float h=std::sqrt(v.x*v.x+v.y*v.y);
    return {std::atan2(-v.z,h)*57.29578f,std::atan2(v.y,v.x)*57.29578f,0.f};
}
