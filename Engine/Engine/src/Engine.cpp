#define _USE_MATH_DEFINES

#include <math/MathAux.h>
#include "Engine.h"
#include <utils/OpenGLUtils.h>
#include "Configurations.h"
#include <view/Transformations.h>
#include "core/Input.h"
#include <imgui/imgui.h>
#include <meshes/MeshLoader.h>
#include <cstdlib>
#include <iostream>
#include <textures/IBL.h>
#include <WarriorRenderer.h>
#include <meshes/MeshGroup.h>
#include "gui/DebugPanel.h"

using namespace WarriorRenderer;

Engine* engine;

///////////////////////////////////////////////////////////////////// CALLBACKS
void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	/*int newWidth, newHeight;
	glfwGetFramebufferSize(win, &newWidth, &newHeight);
	engine->updateWindow(newWidth, newHeight);
	GL_CALL(glViewport(0, 0, newWidth, newHeight));*/
}
void frameBufferSizeCallBack(GLFWwindow* win, int winx, int winy) 
{
	if (winx > 0 && winy > 0)
		engine->updateWindow(winx, winy);
	//engine->updateWindow(2.0f/3.0f * winx, 2.0f / 3.0f * winy);
	//GL_CALL(glViewport(0, winy / 3.0f, 2.0f / 3.0f * winx , 2.0f/3.0f * winy));
}
void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

////////////////////////////////////////////// GLFW
GLFWwindow* setupWindow(int windowWidth, int windowHeight, const char* title,
	int isFullscreen, int isVsync)
{
	GLFWmonitor* monitor = isFullscreen ? glfwGetPrimaryMonitor() : 0;

	/*const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);*/

	GLFWwindow* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title, monitor, NULL);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(isVsync);
	return win;
}
void setupCallbacks(GLFWwindow* win)
{
	glfwSetFramebufferSizeCallback(win, frameBufferSizeCallBack);
	glfwSetWindowSizeCallback(win, window_size_callback);
}
void Engine::setupGLFW() {
	engine = this;
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPEN_GL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPEN_GL_MINOR);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	if (MSAA > 0)
		glfwWindowHint(GLFW_SAMPLES, MSAA);

	windowWidth = SCREEN_WIDTH;
	windowHeight = SCREEN_HEIGHT;
	//windowWidth = 2.0f/3.0f * SCREEN_WIDTH;
	//windowHeight = 2.0f/3.0f * SCREEN_HEIGHT;

	window = setupWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, FULLSCREEN, VSYNC);
	setupCallbacks(window);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
}

////////////////////////////////////////////// GLEW
void Engine::setupGLEW() {
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GL_CALL(GLenum result = glewInit());
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////// SCENE
void Engine::setupScene() {
	editorCamera = new EditorCamera();

	hierarchy = core::Hierarchy::instance;
}

void Engine::setupGUI()
{
	// Setup Dear ImGui context and creating GUI instance
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imGuiIO = ImGui::GetIO();(void)imGuiIO;
	gui = new GUI(imGuiIO, window, *editorCamera);
}

////////////////////////////////////////////// RESOURCES
void Engine::freeResources() {
	delete editorCamera;
	delete gui;
}

////////////////////////////////////////////// SETTERS
void Engine::setPreRender(std::function<void(std::function<void()> renderScene)> preRender) {
	this->preRender = preRender;
}
void Engine::updateWindow(float width, float height)
{
	windowWidth = width;
	windowHeight = height;
	editorCamera->setViewportSize(width, height);
}
void Engine::setSkyBox(const std::vector<std::string>& facesFilePath) {
	if (skybox)
		delete skybox;

	skybox = new SkyBox(facesFilePath, editorCamera);
}

////////////////////////////////////////////// GETTERS
GLFWwindow* Engine::getWindow() {
	return window;
}
EditorCamera* Engine::getCamera() {
	return editorCamera;
}
GUI* Engine::getGui() {
	return gui;
}

int Engine::getWindowWidth() {
	return windowWidth;
}
int Engine::getWindowHeight() {
	return windowHeight;
}
double Engine::getElapsedTime() {
	return elapsedTime;
}


////////////////////////////////////////////// MAIN LOOP
void Engine::run() {

	//renderer3D = new core::Renderer3D();

	// Setup (DO NOT CHANGE ORDER OF SETUP)
	setupGLFW(); 
	
	RenderingConfigurations normalConfigs;
	RenderingConfigurations veryHighConfigs;
	veryHighConfigs.shadowsResolution = 4096;
	Renderer* renderer = new Renderer(veryHighConfigs);
	
	//setupGLEW();  
	
	core::Input::initialize(window);

	//renderer3D->setup();

	//setupScene();
	setupGUI();

	//start();

	//sceneGraph->init(); // Init scene graph after start has been called where the scene setup was made

	/*MeshGroup group2 = MeshGroup::loadFromFile("../Engine/objs/axe.fbx");
	RenderCommand cerberusCommand;
	//group2.meshes[0]->calculateTangents();
	group2.meshes[0]->init();
	cerberusCommand.mesh = group2.meshes[0];
	cerberusCommand.material = renderer->createMaterial();
	cerberusCommand.material->setAlbedoMap(new Texture2D("../Engine/textures/pbr/axe/albedo.png"));
	cerberusCommand.material->setNormalMap(new Texture2D("../Engine/textures/pbr/axe/normal.png"));
	cerberusCommand.material->setMetallicMap(new Texture2D("../Engine/textures/pbr/axe/metallic.png"));
	cerberusCommand.material->setRoughnessMap(new Texture2D("../Engine/textures/pbr/axe/roughness.png"));
	cerberusCommand.material->setAOMap(new Texture2D("../Engine/textures/pbr/axe/ao.png"));
	
	cerberusCommand.model = Mat4::translation({ 0.0f, .5f, 0.0f }) * Mat4::rotation(PI/2, Vec3::Y) * Mat4::rotation(-PI/2, Vec3::X) * Mat4::scaling(2.0f);*/

	MeshGroup group = MeshGroup::loadFromFile("../Engine/objs/sphere.obj");
	RenderCommand renderCommand;
	Mesh* sphereMesh = group.meshes[0];
	sphereMesh->calculateTangents();
	sphereMesh->init();

	WarriorRenderer::Material* material = renderer->createMaterial();
	Texture2D* albedoMap = new Texture2D("../Engine/textures/pbr/plastic/albedo.png");
	Texture2D* normalMap = new Texture2D("../Engine/textures/pbr/plastic/normal.png");
	Texture2D* metallicMap = new Texture2D("../Engine/textures/pbr/plastic/metallic.png");
	Texture2D* roughnessMap = new Texture2D("../Engine/textures/pbr/plastic/roughness.png");
	Texture2D* aoMap = new Texture2D("../Engine/textures/pbr/plastic/ao.png");
	material->setAlbedoMap(albedoMap);
	material->setNormalMap(normalMap);
	material->setMetallicMap(metallicMap);
	material->setRoughnessMap(roughnessMap);
	material->setAOMap(aoMap);

	renderCommand.mesh = sphereMesh;
	renderCommand.material = material;
	renderCommand.model = Mat4::IDENTITY;
	
	RenderCommand renderCommand2;
	Mesh* mesh = MeshLoader::loadFromFile("../Engine/objs/plane.obj");
	mesh->calculateTangents();
	mesh->init();

	renderCommand2.mesh = mesh;
	WarriorRenderer::Material* mat = renderer->createMaterial();
	mat->setAlbedoTint({ 1.0f, 1.0f, 1.0f });
	renderCommand2.material = mat;
	renderCommand2.model = Mat4::translation(0.0f, -1.0f, 0.0f);

	RenderCommand renderCommand3;

	std::vector<RenderCommand> renderCommands;

	WarriorRenderer::Material* defaultMat = renderer->createMaterial();

	for (int j = 0; j < 5; ++j) {
		for (int i = 0; i < 5; ++i) {
		
			RenderCommand rc;

			rc.mesh = sphereMesh;
			rc.model = Mat4::translation(j * 8.5f, 0.01f, i * 8.5f)
				* Mat4::rotation(PI / 2.0f, Vec3::X) * Mat4::scaling(2.0f);;
			rc.material = renderer->createMaterial();
			rc.material->setMetallic(i / 4.0f);
			rc.material->setRoughness(j / 4.0f);
			rc.material->setAlbedoTint({ i/4.0f, i / 4.0f, i / 4.0f });

			renderCommands.push_back(rc);
		}

	}
	
	Lights lights;
	DirectionalLight light;
	DirectionalLight light2;
	light2.direction = { 10.0, -10.0, 0.0 };
	light2.color = { 1.0, 1.0, 1.0};
	lights.directionalLights.push_back(light);
	lights.directionalLights.push_back(light2);

	
	/*for (int i = -10; i < 10; ++i) {
		for (int j = -10; j < 10; ++j) {
			PointLight pLight;
			pLight.position = { (float)i * 3.5f ,0.0f , (float)j * 3.5f };
			pLight.color = { 1.0f ,fabsf(i)/10.0f , fabsf(j)/10.0f };
			pLight.radiance = 50.0f;
			pLight.radius = 2.0f + sinf(i) + cos(j);
			lights.pointLights.push_back(pLight);
		}
	}*/
	
	editorCamera = new EditorCamera();

	FxaaCommand fxaa;
	ACESToneMappingCommand toneMapping;

	editorCamera->setEditorWindowFocus(true);

	float rotation = PI / 4.0f;
	Vec3 translation(-2.0f, 2.0f, 2.0f);

	/*IBL::ComputeIrradianceCubeMap("../Engine/textures/hdr/default/default.hdr",
		"../Engine/textures/irradiance2/");*/

	double lastTime = glfwGetTime();

	gui->getMaterialPanel().showMaterial(material);

	DebugPanel debugPanel(renderer);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		double time = glfwGetTime();
		elapsedTime = time - lastTime;
		lastTime = time;

		gui->preRenderUI();

		//gui->getMaterialPanel().onGUI();

		editorCamera->update(elapsedTime);

		//ImGui::ShowMetricsWindow();

		//renderer->enqueueRender(&renderCommand2);
		//renderer->enqueueRender(&cerberusCommand);

		for (RenderCommand& rc : renderCommands) {
			renderer->enqueueRender(&rc);
		}

		static bool enableToneMapping = true;
		ImGui::Checkbox("Tone Mapping", &enableToneMapping);
		if (enableToneMapping) renderer->enqueuePostProcessing(&toneMapping);

		static bool enableFxaa = true;
		ImGui::Checkbox("Fxaa", &enableFxaa);
		if (enableFxaa) renderer->enqueuePostProcessing(&fxaa);

		renderer->render(*editorCamera, lights);

		//ImGui::Image((ImTextureID)renderer->getBufferTexture(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

		debugPanel.onGUI();

		//update();

		//bool open = false;
		//ImGui::ShowDemoWindow(&open);

		//hierarchy->updateScene();

		//renderer3D->renderToFrameBuffer(*editorCamera, *hierarchy, editorCamera->getFrameBuffer());

		//gui->renderUI(*editorCamera);

		gui->postRenderUI();

		glfwSwapBuffers(window);
		checkForOpenGLErrors("ERROR: MAIN LOOP"); //TODO Prob not necessary
	}

	// Cleanup
	//freeResources();
	//end(); //Has to be called before glfwTerminate()
	
	core::Input::terminate();

	glfwDestroyWindow(window);
	glfwTerminate();
}

