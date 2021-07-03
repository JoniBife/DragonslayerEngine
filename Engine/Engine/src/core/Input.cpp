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

Vec2 core::Input::getMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(instance->window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}
