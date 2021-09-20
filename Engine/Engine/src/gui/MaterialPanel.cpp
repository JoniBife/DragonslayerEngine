#include "MaterialPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

MaterialPanel::MaterialPanel()
{
}

void MaterialPanel::showMaterial(Material* material)
{
	selectedMaterial = material;
}

void MaterialPanel::stopShowingMaterial()
{
	selectedMaterial = nullptr;
}

void MaterialPanel::onGUI()
{
	ImGui::Begin("Material");

	if (selectedMaterial != nullptr) {

		//selectedMaterial->onGUI();
	}

	ImGui::End();
}
