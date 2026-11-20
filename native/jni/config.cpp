#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
Config Config::load(const std::string& p){
    Config c; std::ifstream f(p); if(!f) return c;
    try{ json j; f>>j;
        c.aim_enabled=j.value("aim_enabled",c.aim_enabled);
        c.aim_fov=j.value("aim_fov",c.aim_fov);
        c.aim_smooth=j.value("aim_smooth",c.aim_smooth);
        c.aim_bone=j.value("aim_bone",c.aim_bone);
        c.aim_range=j.value("aim_range",c.aim_range);
        c.bhop_enabled=j.value("bhop_enabled",c.bhop_enabled);
        c.esp_enabled=j.value("esp_enabled",c.esp_enabled);
        c.esp_box=j.value("esp_box",c.esp_box);
        c.esp_name=j.value("esp_name",c.esp_name);
        c.esp_health=j.value("esp_health",c.esp_health);
        c.esp_skel=j.value("esp_skel",c.esp_skel);
        c.wh_enabled=j.value("wh_enabled",c.wh_enabled);
        c.snow_enabled=j.value("snow_enabled",c.snow_enabled);
    }catch(...){}
    return c;
}
void Config::save(const std::string& p) const{
    json j={{"aim_enabled",aim_enabled},{"aim_fov",aim_fov},{"aim_smooth",aim_smooth},
        {"aim_bone",aim_bone},{"aim_range",aim_range},{"bhop_enabled",bhop_enabled},
        {"esp_enabled",esp_enabled},{"esp_box",esp_box},{"esp_name",esp_name},
        {"esp_health",esp_health},{"esp_skel",esp_skel},{"wh_enabled",wh_enabled},
        {"snow_enabled",snow_enabled}};
    std::ofstream f(p); f<<j.dump(2);
}
