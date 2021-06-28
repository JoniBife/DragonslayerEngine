#include "Transform.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

core::Transform::Transform()
{
}

void core::Transform::onGUI() const
{
	ImGui::Text("Position");
	ImGui::TextColored(ImVec4(1,0,0,1), "X"); ImGui::SameLine();

	Vec3 pos = this->position;
	ImGui::InputFloat("##label", &(pos.x)); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0,1,0,1), "Y"); ImGui::SameLine();
	ImGui::InputFloat("##label", &(pos.y)); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0,0,1,1), "Z"); ImGui::SameLine();
	ImGui::InputFloat("##label", &(pos.z)); ImGui::SameLine();

	ImGui::Separator();

	ImGui::Text("Rotation");
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();

	ImGui::Separator();

	ImGui::Text("Scale");
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
}
