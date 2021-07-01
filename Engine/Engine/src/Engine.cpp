#define _USE_MATH_DEFINES
#include <math.h>
#include "math/MathAux.h"
#include "Engine.h"
#include "utils/OpenGLUtils.h"
#include "Configurations.h"
#include "view/Transformations.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

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

////////////////////////////////////////////// OPENGL
void printOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cout << "OpenGL version " << version << std::endl;
	std::cout << "GLSL version " << glslVersion << std::endl;
}
void Engine::setupOpenGL() {
#if _DEBUG
	printOpenGLInfo();
#endif
	GL_CALL(glClearColor(BACKGROUND_COLOR));
	GL_CALL(glEnable(GL_DEPTH_TEST));
	GL_CALL(glDepthFunc(GL_LEQUAL));
	GL_CALL(glDepthMask(GL_TRUE));
	GL_CALL(glDepthRange(0.0, 1.0));
	GL_CALL(glClearDepth(1.0));
	GL_CALL(glEnable(GL_CULL_FACE));
	GL_CALL(glCullFace(GL_BACK));
	GL_CALL(glFrontFace(GL_CCW));
	if (MSAA > 0) GL_CALL(glEnable(GL_MULTISAMPLE));
	GL_CALL(glViewport(0, SCREEN_HEIGHT - windowHeight, windowWidth, windowHeight));
}

////////////////////////////////////////////// SCENE
void Engine::setupScene() {


	editorCamera = new Camera();

	sceneGraph = new SceneGraph(editorCamera);

	hierarchy = core::Hierarchy::instance;
}

void Engine::setupGUI()
{
	// Setup Dear ImGui context and creating GUI instance
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imGuiIO = ImGui::GetIO();(void)imGuiIO;
	gui = new GUI(imGuiIO, window);
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
Camera* Engine::getCamera() {
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

	renderer3D = new core::Renderer3D();

	// Setup (DO NOT CHANGE ORDER OF SETUP)
	setupGLFW(); 
	setupGLEW();  
	
	renderer3D->setup();

	setupGUI();
	setupScene();

	start();
	//sceneGraph->init(); // Init scene graph after start has been called where the scene setup was made

	double lastTime = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		double time = glfwGetTime();
		elapsedTime = time - lastTime;
		lastTime = time;

		update();

		editorCamera->update(elapsedTime);

		hierarchy->updateScene();

		Texture2D& frameTexture = renderer3D->RenderToTexture(*editorCamera, *hierarchy);
		gui->drawUI(frameTexture.getId(), *editorCamera);// After everything from the scene is rendered, we render the UI;

		glfwSwapBuffers(window);
		checkForOpenGLErrors("ERROR: MAIN LOOP"); //TODO Prob not necessary
	}

	// Cleanup
	freeResources();
	end(); //Has to be called before glfwTerminate()

	glfwDestroyWindow(window);
	glfwTerminate();
}

