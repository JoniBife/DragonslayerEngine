#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include <stdio.h>

class GUI {

private: 
	ImGuiIO& imGuiIO;

	void loadFonts();

public:
	GUI(ImGuiIO& imGuiIO, GLFWwindow* window);
	~GUI();

	void drawUI();
};

#endif
