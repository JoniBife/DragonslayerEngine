#include "Input.h"

using namespace core;

Input::Input(GLFWwindow* window) : window(window) {
	// There can only be one instance of input
	assert(instance == nullptr);
}

Input::~Input() { }

Input* Input::instance = nullptr;

void Input::initialize(GLFWwindow* window) {
	assert(instance == nullptr);
	instance = new Input(window);
}

void Input::terminate() {
	assert(instance != nullptr);
	delete instance;
}

bool Input::isKeyDown(const KeyCode keyCode) {
	
	int state = glfwGetKey(instance->window, static_cast<int>(keyCode));
	return state == GLFW_PRESS;
}

bool core::Input::isKeyUp(const KeyCode keyCode)
{
	int state = glfwGetKey(instance->window, static_cast<int>(keyCode));
	return state == GLFW_RELEASE;
}

bool core::Input::isMouseButtonDown(const MouseButtonCode mouseButtonCode)
{
	int state = glfwGetMouseButton(instance->window, static_cast<int>(mouseButtonCode));
	return state == GLFW_PRESS;
}

bool core::Input::isMouseButtonUp(const MouseButtonCode mouseButtonCode)
{
	int state = glfwGetMouseButton(instance->window, static_cast<int>(mouseButtonCode));
	return state == GLFW_RELEASE;
}

void core::Input::setCursorVisibility(bool visible)
{
	if (instance->cursorVisible != visible) {

		instance->cursorVisible = visible;

		if (!visible) {
			/*
			This will hide the cursor and lock it to the specified window. 
			GLFW will then take care of all the details of cursor re-centering and offset calculation 
			and providing the application with a virtual cursor position.
			This virtual position is provided normally via both the cursor position callback and through polling.
			*/
			glfwSetInputMode(instance->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(instance->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

Vec2 core::Input::getMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(instance->window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}


