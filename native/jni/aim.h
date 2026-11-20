#pragma once
#include "vmath.h"
struct PlayerSnapshot;
bool aim_best(const PlayerSnapshot*,int,const PlayerSnapshot&,const float[16],float,float,float,float,int,Vec3&);
void aim_apply(class Il2Cpp&,void*,size_t,Vec3,float);
