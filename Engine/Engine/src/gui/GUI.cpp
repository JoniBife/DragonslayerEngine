#include "TextRenderer.h"
#include "UIRenderer.h"
#include "LineRenderer.h"
#include "GUI.h"
#include "../view/Transformations.h"

#define ever (;;)

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width > 0 && height > 0) {
		Mat4 projection = ortho(0.0f, float(width), 0.0f, float(height));

		TextRenderer::getInstance()->updateProjection(projection);
		UIRenderer::getInstance()->updateProjection(projection);
		LineRenderer::getInstance()->updateProjection(projection);
	}
}


GUI::GUI(GLFWwindow* window) : window(window) {
	// Setting a callback to scale the UI when the window framebuffer size changes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

GUI::~GUI() {
	for (GUIComponent* comp : guiComponents)
		delete comp;
}


void GUI::drawUI() {
	/*
	* We are using the painters algorithm to draw the GUI.
	* Each component is associated with a depth and the components
	* are drawn from the furthest to the closest.
	* Since the insertion is sorted we don't need to sort the list each
	* time we are drawing.
	*/
	for (GUIComponent* comp : guiComponents) {
		comp->draw();
	}
}

void GUI::removeComponent(GUIComponent* component) {
	guiComponents.remove(component);
}

void GUI::addComponent(GUIComponent* component) {
	component->setWindow(window);
	addComponentSort(component);
}

void GUI::addComponentSort(GUIComponent* component) {

	// If the list is empty we insert in the front
	if (guiComponents.empty()) {
		guiComponents.push_front(component);
		return;
	}

	// Otherwise we find the adequate position to insert
	std::list<GUIComponent*>::iterator curr = guiComponents.begin();

	for ever {

		// If we reached the end of the list we insert in the end
		if (curr == guiComponents.end()) {
			guiComponents.push_back(component);
			return;
		}

		// If the depth is smaller or equal to the curr then we insert before it
		if (component->getDepth() <= (**curr).getDepth()) {
			guiComponents.insert(curr, component);
			return;
		}
;
		++curr;
	}
}

