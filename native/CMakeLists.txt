cmake_minimum_required(VERSION 3.20)
project(gepstorm CXX)
set(CMAKE_CXX_STANDARD 20)
add_library(gepstorm SHARED
    jni/gepstorm_main.cpp jni/hook.cpp jni/il2cpp_resolver.cpp
    jni/config.cpp jni/esp.cpp jni/aim.cpp jni/bhop.cpp
    jni/snow.cpp jni/overlay_gl.cpp jni/auth_bridge.cpp)
target_include_directories(gepstorm PRIVATE jni
    third_party/imgui third_party/imgui/backends
    third_party/dobby/include third_party/nlohmann)
add_subdirectory(third_party/imgui ${CMAKE_BINARY_DIR}/imgui_build)
add_subdirectory(third_party/Dobby ${CMAKE_BINARY_DIR}/dobby_build)
target_link_libraries(gepstorm log android GLESv3 EGL imgui dobby)
target_compile_options(gepstorm PRIVATE -O2 -fvisibility=hidden -Wno-unused-parameter)
