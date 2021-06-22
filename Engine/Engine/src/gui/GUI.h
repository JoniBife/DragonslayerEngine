#ifndef GUI_H
#define GUI_H

#include <list>
#include "GUIComponent.h"

class GUI {

private: 
	// Doubly-linked list, the list is sorted from the largest depth to the smallest depth 
	std::list<GUIComponent*> guiComponents;
	GLFWwindow* window;

public:
	GUI(GLFWwindow* window);
	~GUI();

	void drawUI();

	void removeComponent(GUIComponent* component);

	void addComponent(GUIComponent* component);

private:
	void addComponentSort(GUIComponent* component);
};

#endif
