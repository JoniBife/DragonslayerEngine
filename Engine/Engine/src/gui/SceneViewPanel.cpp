#include "SceneViewPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

SceneViewPanel::SceneViewPanel(Camera& editorCamera, HierarchyPanel& hierarchyPanel) : editorCamera(editorCamera), hierarchyPanel(hierarchyPanel)
{
}

void SceneViewPanel::onGUI()
{
	ImGui::Begin("SceneView");
	{
		ImGui::BeginChild("SceneRenderer");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		editorCamera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);

		ImVec2 wsize = ImGui::GetWindowSize();

		//ImGui::Image((ImTextureID)id, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}
