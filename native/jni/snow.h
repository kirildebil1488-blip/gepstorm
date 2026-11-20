#pragma once
#include "imgui.h"
struct Flake{float x,y,vx,vy,r,ph;};
class Snow{
public: void init(int,int,int=200); void update(float,int,int); void render(ImDrawList*,int,int);
private: static constexpr int MAX=512; Flake f_[MAX]; int n_=0;
};
