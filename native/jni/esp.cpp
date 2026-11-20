#include "esp.h"
#include <algorithm>
static void corner_box(ImDrawList* dl,Vec2 tl,Vec2 br,ImU32 c){
    dl->AddRect({tl.x,tl.y},{br.x,br.y},c,0.f,0,1.4f);
    float cx=(br.x-tl.x)*0.25f,cy=(br.y-tl.y)*0.25f;
    dl->AddLine({tl.x,tl.y},{tl.x+cx,tl.y},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({tl.x,tl.y},{tl.x,tl.y+cy},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({br.x,tl.y},{br.x-cx,tl.y},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({br.x,tl.y},{br.x,tl.y+cy},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({tl.x,br.y},{tl.x+cx,br.y},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({tl.x,br.y},{tl.x,br.y-cy},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({br.x,br.y},{br.x-cx,br.y},IM_COL32(255,255,255,220),2.f);
    dl->AddLine({br.x,br.y},{br.x,br.y-cy},IM_COL32(255,255,255,220),2.f);
}
void esp_render(ImDrawList* dl,const PlayerSnapshot* ps,int n,const PlayerSnapshot& l,
                const float vm[16],float W,float H,bool box,bool name,bool health,bool skel){
    for(int i=0;i<n;++i){
        const auto& p=ps[i]; if(!p.alive||p.team==l.team) continue;
        Vec2 f,h; if(!w2s(vm,p.pos,W,H,f)) continue; if(!w2s(vm,p.head,W,H,h)) continue;
        float bh=f.y-h.y; if(bh<6.f||bh>H) continue; float bw=bh*0.45f;
        Vec2 tl{h.x-bw*0.5f,h.y}, br{h.x+bw*0.5f,f.y};
        ImU32 col=IM_COL32(255,60,60,235);
        if(box) corner_box(dl,tl,br,col);
        if(health){ float r=std::clamp(p.hp/100.f,0.f,1.f);
            dl->AddRectFilled({tl.x-5,br.y-(br.y-tl.y)*r},{tl.x-1,br.y},IM_COL32(0,255,120,200)); }
        if(name){ char b[64]; snprintf(b,sizeof(b),"p%d [%d]",i,p.hp);
            dl->AddText({tl.x,tl.y-16},IM_COL32(255,255,255,230),b); }
        if(skel){ Vec2 m{(h.x+f.x)*0.5f,(h.y+f.y)*0.5f};
            dl->AddLine({h.x,h.y},{m.x,m.y},col,1.2f); dl->AddLine({m.x,m.y},{f.x,f.y},col,1.2f); }
    }
}
