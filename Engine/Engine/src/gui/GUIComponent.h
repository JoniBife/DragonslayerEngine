#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "GLFW/glfw3.h"

class GUIComponent {

private:
	unsigned int depth = 0;

protected:
	GLFWwindow* window;

public:

	unsigned int getDepth() const;

	void setDepth(unsigned int depth);

	void setWindow(GLFWwindow* window);

	virtual void draw() = 0;

};

#endif