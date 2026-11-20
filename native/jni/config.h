#pragma once
#include <string>
struct Config{
    bool aim_enabled=true; float aim_fov=220.f,aim_smooth=2.5f,aim_range=300.f; int aim_bone=0;
    bool bhop_enabled=true;
    bool esp_enabled=true,esp_box=true,esp_name=true,esp_health=true,esp_skel=true,wh_enabled=true;
    bool snow_enabled=true;
    static Config load(const std::string&); void save(const std::string&) const;
};
