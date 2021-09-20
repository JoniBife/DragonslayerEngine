#ifndef GUI_H
#define GUI_H

#include "HierarchyPanel.h"
#include "ObjectPanel.h"
#include "SceneViewPanel.h"
#include "MaterialPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#include <view/Camera.h>

class GUI {

private: 
	ImGuiIO& imGuiIO;
	HierarchyPanel* hierarchyPanel = nullptr;
	ObjectPanel* objectPanel = nullptr;
	SceneViewPanel* sceneViewPanel = nullptr;
	MaterialPanel* materialPanel = nullptr;
	
	void setDefaultTheme();
	void setDefaultStyle();
	void loadFonts();

public:
	GUI(ImGuiIO& imGuiIO, GLFWwindow* window, EditorCamera& editorCamera);
	~GUI();

	void preRenderUI();
	void renderUI(Camera& camera);
	void postRenderUI();

	MaterialPanel& getMaterialPanel() const;
};

#endif
