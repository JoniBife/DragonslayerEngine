#include "Transform.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../math/MathAux.h"
#include "../math/Qtrn.h"

using namespace core;

Transform::Transform()
{
}

Mat4 Transform::getModelMatrix()
{
	return model;
}

void Transform::onGUI()
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

	Vec3 rotationInDegrees = { 
		radiansToDegrees(rotation.x), 
		radiansToDegrees(rotation.y),
		radiansToDegrees(rotation.z)
	};

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationX", &(rotationInDegrees.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationY", &(rotationInDegrees.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##RotationZ", &(rotationInDegrees.z));

	rotation.x = degreesToRadians(rotationInDegrees.x);
	rotation.y = degreesToRadians(rotationInDegrees.y);
	rotation.z = degreesToRadians(rotationInDegrees.z);

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

void Transform::onFrameUpdate(const Mat4& parentModel)
{
	Mat4 qtrnRotation = (Qtrn(rotation.z, Vec3::Z) * Qtrn(rotation.y, Vec3::Y) * Qtrn(rotation.x, Vec3::X)).toRotationMatrix();

	/*model = Mat4::translation(position) * Mat4::rotation(rotation.z, Vec3::Z) *
		Mat4::rotation(rotation.y, Vec3::Y) *
		Mat4::rotation(rotation.x, Vec3::X) *
		Mat4::scaling(scale);*/

	// TODO Create quaternion method to calculate the rotation from yaw pitch and roll

	model = Mat4::translation(position) * qtrnRotation * Mat4::scaling(scale);

	model = parentModel * model;
}

void Transform::makeChildOfTransform(Transform* transform)
{
	this->position += transform->position;
	this->rotation += transform->rotation;
	this->scale *= transform->scale;
}

void Transform::removeChildOfTransform(Transform* transform)
{
	this->position -= transform->position;
	this->rotation -= transform->rotation;
	this->scale /= transform->scale;
}
