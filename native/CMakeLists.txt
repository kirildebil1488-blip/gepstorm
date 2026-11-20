cmake_minimum_required(VERSION 3.20)
project(gepstorm CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_android.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_library(gepstorm SHARED
    jni/gepstorm_main.cpp jni/hook.cpp jni/il2cpp_resolver.cpp
    jni/config.cpp jni/esp.cpp jni/aim.cpp jni/bhop.cpp
    jni/snow.cpp jni/overlay_gl.cpp jni/auth_bridge.cpp)
target_include_directories(gepstorm PRIVATE jni third_party/nlohmann ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(gepstorm log android GLESv3 EGL imgui)
target_compile_options(gepstorm PRIVATE -O2 -fvisibility=hidden -Wno-unused-parameter)
