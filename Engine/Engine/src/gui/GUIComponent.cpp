#include "GUIComponent.h"

unsigned int GUIComponent::getDepth() const {
	return depth;
}

void GUIComponent::setDepth(unsigned int depth) {
	this->depth = depth;
}

void GUIComponent::setWindow(GLFWwindow* window) {
	this->window = window;
}