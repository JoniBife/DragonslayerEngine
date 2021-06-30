#include "Transform.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

core::Transform::Transform()
{
}

Mat4 core::Transform::getModelMatrix()
{
	// TODO Move this to somewhere more intuitive
	model = Mat4::translation(position) * Mat4::rotation(rotation.z, Vec3::Z) *
		Mat4::rotation(rotation.y, Vec3::Y) *
		Mat4::rotation(rotation.x, Vec3::X) *
		Mat4::scaling(scale);

	return model;
}

void core::Transform::onGUI()
{
	float inputFieldsWidth = ImGui::GetContentRegionAvail().x * 0.25f;

	ImGui::Text("Position");

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionX", &(position.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionY", &(position.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionZ", &(position.z));
	

	ImGui::Text("Rotation");

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationX", &(rotation.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationY", &(rotation.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationZ", &(rotation.z));
	

	ImGui::Text("Scale");

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##ScaleX", &(scale.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##ScaleY", &(scale.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##ScaleZ", &(scale.z)); 
	
}

void core::Transform::update(const Mat4& parentModel)
{
	position = parentModel * position;
	rotation = parentModel * rotation;
	scale = parentModel * scale;
}
