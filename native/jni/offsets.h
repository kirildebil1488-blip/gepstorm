#pragma once
#include <cstdint>
#include <string>
namespace off {
    inline const std::string GAME_LIB="libil2cpp.so";
    inline const char* IL2CPP_DOMAIN_GET="il2cpp_domain_get";
    inline const char* IL2CPP_DOMAIN_ASSEMBLY="il2cpp_domain_assembly_open";
    inline const char* IL2CPP_CLASS_FROM_NAME="il2cpp_class_from_name";
    inline const char* IL2CPP_FIELD_FROM_NAME="il2cpp_class_get_field_from_name";
    inline const char* IL2CPP_FIELD_OFFSET="il2cpp_field_get_offset";
    inline const char* IL2CPP_ASM_IMAGE="il2cpp_assembly_get_image";
    inline const char* NS_GAME="";
    inline const char* CLS_PLAYER="Player";
    inline const char* CLS_WORLD="World";
    inline const char* F_POSITION="position";
    inline const char* F_HEAD="headPosition";
    inline const char* F_TEAM="team";
    inline const char* F_HEALTH="health";
    inline const char* F_ALIVE="isAlive";
    inline const char* F_VELOCITY="velocity";
    inline const char* F_GROUNDED="isGrounded";
    inline const char* F_VIEWANGL="viewAngles";
    inline const char* F_PLAYER_LIST="players";
    inline const char* F_LOCAL_PLAYER="localPlayer";
    inline const char* F_VIEW_MATRIX="worldToCameraMatrix";
}
