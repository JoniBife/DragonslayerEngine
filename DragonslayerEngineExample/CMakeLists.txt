cmake_minimum_required (VERSION 3.8)

project(DragonslayerEngineExample)

# SETUP GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory ("libs/glfw")

# SETUP IMGUI
add_library(imgui STATIC
		"libs/imgui/imgui.cpp"
		"libs/imgui/imgui_draw.cpp"
		"libs/imgui/imgui_tables.cpp"
		"libs/imgui/imgui_widgets.cpp"
		"libs/imgui/imgui_demo.cpp"
		"libs/imgui/backends/imgui_impl_glfw.cpp"
		"libs/imgui/backends/imgui_impl_opengl3.cpp"
		)
target_include_directories(imgui PUBLIC "libs/imgui" "libs/imgui/backends")
target_link_libraries(imgui glfw)

# SETUP RENDERER
set(GENERATE_DEBUG_INFORMATION ON CACHE BOOL "" FORCE)

# SETUP EXAMPLE
add_executable (DragonslayerEngineExample "Main.cpp"  "cameras/EditorCamera.cpp" "cameras/EditorCamera.h" "core/Input.cpp" "core/Input.h")
target_include_directories(DragonslayerEngineExample PUBLIC "")
target_link_libraries (DragonslayerEngineExample
	glfw
	LMath
	imgui
	DragonslayerEngine
)