#include "InputManager.h"
#include <iostream>
#include <assert.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager* im = InputManager::getInstance();

	if (!im->receivingTextInput) {
		if (action == GLFW_PRESS)
			im->keysPressed[key] = true;
		else if (action == GLFW_RELEASE)
			im->keysPressed[key] = false;
	}
	else {
		if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS || action == GLFW_REPEAT) {
			if (!im->textInput.empty())
				im->textInput.pop_back();
		} 
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	InputManager* im = InputManager::getInstance();

	// We ignore characters outside the first 128 of the ascii table
	if (im->receivingTextInput && codepoint < 128) {
		im->textInput.push_back(char(codepoint));
	}
}

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(GLFWwindow* window) {
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
}

void InputManager::startTextInput(int inputReceiverId) {
	assert(inputReceiverId < currInputReceiverId);
	this->inputReceiverId = inputReceiverId;
	receivingTextInput = true;
	textInput.clear();
}
void InputManager::resumeTextInput(int inputReceiverId, const std::string& oldInput) {
	assert(inputReceiverId < currInputReceiverId);
	this->inputReceiverId = inputReceiverId;
	receivingTextInput = true;
	textInput = oldInput;
}
void InputManager::stopTextInput(int inputReceiverId) {
	assert(inputReceiverId < currInputReceiverId);
	if (this->inputReceiverId == inputReceiverId) {
		receivingTextInput = false;
		textInput.clear();
	}
}
std::string InputManager::getCurrTextInput() const {
	return textInput;
}

bool InputManager::isKeyPressed(int key) {

	// While we are receiving text input, we ignore all other kinds of input
	if (receivingTextInput)
		return false;
	return keysPressed[key];
}

int InputManager::generateInputReceiverId() {
	int id = currInputReceiverId;
	++currInputReceiverId;
	return id;
}

void InputManager::createInstance(GLFWwindow* window) {
	if (instance)
		return;

	instance = new InputManager(window);
}

InputManager* InputManager::getInstance() {
	if (instance)
		return instance;
	std::cout << "InputManager ERROR [ Instance not created ! ]." << std::endl;
	exit(EXIT_FAILURE);
}

void InputManager::destroyInstance() {
	if (instance)
		delete instance;
}

