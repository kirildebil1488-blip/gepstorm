#pragma once
#include "imgui.h"
#include "vmath.h"
struct PlayerSnapshot{Vec3 pos,head; int team,hp; bool alive;};
void esp_render(ImDrawList*,const PlayerSnapshot*,int,const PlayerSnapshot&,const float[16],float,float,bool,bool,bool,bool);
