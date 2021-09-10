#define _USE_MATH_DEFINES
#include <math.h>
#include "math/MathAux.h"
#include "Engine.h"
#include "utils/OpenGLUtils.h"
#include "Configurations.h"
#include "view/Transformations.h"
#include "core/Input.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#include "meshes/MeshLoader.h"
#include "gui/ImGuiExtensions.h"
#include <cstdlib>
#include "textures/IBL.h"

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
	engine->updateWindow(winx, winy);
	GL_CALL(glViewport(0, 0, winx, winy));
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

	sceneGraph = new SceneGraph(editorCamera);

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
	delete sceneGraph;
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
SceneGraph* Engine::getSceneGraph() {
	return sceneGraph;
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
	DeferredRenderPipeline* deferredRenderPipeline = new DeferredRenderPipeline();
	
	//setupGLEW();  
	
	core::Input::initialize(window);

	//renderer3D->setup();

	//setupScene();
	setupGUI();

	//start();

	//sceneGraph->init(); // Init scene graph after start has been called where the scene setup was made

	RenderCommand renderCommand;
	Mesh* sphereMesh = MeshLoader::loadFromFile("../Engine/objs/sphere.obj");
	sphereMesh->calculateTangents();
	sphereMesh->init();

	GLPBRMaterial* material = deferredRenderPipeline->createMaterial();
	Texture2D* albedoMap = new Texture2D("../Engine/textures/pbr/rustediron/albedo.png");
	Texture2D* normalMap = new Texture2D("../Engine/textures/pbr/rustediron/normal.png");
	Texture2D* metallicMap = new Texture2D("../Engine/textures/pbr/rustediron/metallic.png");
	Texture2D* roughnessMap = new Texture2D("../Engine/textures/pbr/rustediron/roughness.png");
	Texture2D* aoMap = new Texture2D("../Engine/textures/pbr/rustediron/ao.png");
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
	renderCommand2.material = deferredRenderPipeline->createMaterial();
	renderCommand2.model = Mat4::translation(0.0f, -1.0f, 0.0f);

	RenderCommand renderCommand3;
	Mesh* mesh2 = MeshLoader::loadFromFile("../Engine/objs/cylinder64.obj");
	mesh2->calculateTangents();
	mesh2->init();

	GLPBRMaterial* material2 = deferredRenderPipeline->createMaterial();
	albedoMap = new Texture2D("../Engine/textures/pbr/rustediron/albedo.png");
	normalMap = new Texture2D("../Engine/textures/pbr/rustediron/normal.png");
	metallicMap = new Texture2D("../Engine/textures/pbr/rustediron/metallic.png");
	roughnessMap = new Texture2D("../Engine/textures/pbr/rustediron/roughness.png");
	aoMap = new Texture2D("../Engine/textures/pbr/rustediron/ao.png");
	material2->setAlbedoMap(albedoMap);
	material2->setNormalMap(normalMap);
	material2->setMetallicMap(metallicMap);
	material2->setRoughnessMap(roughnessMap);
	material2->setAOMap(aoMap);

	renderCommand3.mesh = mesh2;
	renderCommand3.material = material2;
	renderCommand3.model = Mat4::translation(-2.0f, 2.0f, 2.0f) * Mat4::rotation(PI / 4.0f, Vec3::Z);

	std::vector<RenderCommand> renderCommands;

	for (int i = -2; i < 10; ++i) {
		RenderCommand rc;

		if (i < 0)
			rc.castShadows = false;

		rc.mesh = sphereMesh;
		rc.model = Mat4::translation(0, 1.0f, -i * 3.5f); //* Mat4::scaling(rand() % 2 + 1 * 0.5f, rand() % 2 + 1 * 0.5f, rand() % 2 + 1 * 0.5f);
		rc.material = material;

		renderCommands.push_back(rc);
	}
	
	Lights lights;
	DirectionalLight light;
	lights.directionalLights.push_back(light);

	editorCamera = new EditorCamera();

	double lastTime = glfwGetTime();

	editorCamera->setEditorWindowFocus(true);

	float rotation = PI / 4.0f;
	Vec3 translation(-2.0f, 2.0f, 2.0f);

	IBL::ComputeIrradianceCubeMap("../Engine/textures/hdr/Hamarikyu_Bridge_B/14-Hamarikyu_Bridge_B_3k.hdr", 
		"../Engine/textures/irradiance/");

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		double time = glfwGetTime();
		elapsedTime = time - lastTime;
		lastTime = time;

		gui->preRenderUI();

		editorCamera->update(elapsedTime);

		ImGui::ShowMetricsWindow();
		ImGui::InputVec3("Translation", translation);
		ImGui::InputVec3("Translation2", translation);
		ImGui::InputFloat("Rotation", &rotation);
		ImGui::InputVec3("Light Direction", lights.directionalLights[0].direction);

		//deferredRenderPipeline->enqueueRender(renderCommand);
		deferredRenderPipeline->enqueueRender(renderCommand2);
		//deferredRenderPipeline->enqueueRender(renderCommand3);

		for (RenderCommand rc : renderCommands) {
			deferredRenderPipeline->enqueueRender(rc);
		}

		deferredRenderPipeline->render(*editorCamera, lights);

		rotation += (PI / 6.0f) * elapsedTime;

		

		renderCommand3.model = Mat4::translation(translation) * Mat4::rotation(rotation, Vec3::Z);

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

