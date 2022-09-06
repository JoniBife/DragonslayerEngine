cmake_minimum_required (VERSION 3.8)

project(DragonslayerEngineExample)

add_executable (DragonslayerEngineExample "Main.cpp"  "cameras/EditorCamera.cpp" "cameras/EditorCamera.h" "core/Input.cpp" "core/Input.h")

target_include_directories(DragonslayerEngineExample PUBLIC "")

add_subdirectory ("libs/glfw")

target_link_libraries (DragonslayerEngineExample
        glfw
        LMath
        DragonslayerEngine
)