cmake_minimum_required (VERSION 3.8)

project(DragonslayerEngineExample)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory ("libs/glfw")

add_executable (DragonslayerEngineExample "Main.cpp"  "cameras/EditorCamera.cpp" "cameras/EditorCamera.h" "core/Input.cpp" "core/Input.h")

target_include_directories(DragonslayerEngineExample PUBLIC "")

target_link_libraries (DragonslayerEngineExample
	glfw
	LMath
	DragonslayerEngine
)