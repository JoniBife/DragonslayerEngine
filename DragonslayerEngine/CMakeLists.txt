cmake_minimum_required (VERSION 3.8)

project (DragonslayerEngine VERSION 1.0.0 DESCRIPTION "A 3D real-time rendering engine suitable for games and 3D applications.")

add_subdirectory ("libs/glad")
add_subdirectory ("libs/assimp")
add_subdirectory ("libs/lmath")

add_library (DragonslayerEngine STATIC
	"src/Renderer.cpp"
	"src/GLObject.cpp"
	"src/RenderTarget.cpp"
	"src/RenderQueue.cpp"
	"src/RenderingConfigurations.cpp"
	"src/RenderCommand.cpp"
	"src/PostProcessingCommand.cpp"
	"src/OpenGLState.cpp"
	"src/Material.cpp"
	"src/view/Camera.cpp"
	"src/view/Transformations.cpp"
	"src/utils/OpenGLUtils.cpp"
	"src/textures/CubeMap.cpp"
	"src/textures/FrameBuffer.cpp"
	"src/textures/Texture2D.cpp"
	"include/DragonslayerEngine/textures/FloatArrayFile.h"
	"include/DragonslayerEngine/textures/IBL.h"
	"src/textures/stb_image.h"
	"src/textures/stb_image_write.h"
	"src/shaders/Shader.cpp"
	"src/shaders/ShaderProgram.cpp"
	"src/meshes/Mesh.cpp"
	"src/meshes/MeshGroup.cpp"
	"src/meshes/MeshLoader.cpp"
	"src/meshes/SkyBox.cpp"
)

target_link_libraries (DragonslayerEngine
		glad
		assimp
		LMath
		)

target_include_directories(DragonslayerEngine PUBLIC "include")

add_custom_command(TARGET DragonslayerEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/deferred $<TARGET_FILE_DIR:DragonslayerEngine>/shaders/deferred
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/skybox $<TARGET_FILE_DIR:DragonslayerEngine>/shaders/skybox
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:DragonslayerEngine>/assets
		)
