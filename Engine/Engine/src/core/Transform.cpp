#include "Transform.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#include <math/MathAux.h>
#include <math/Qtrn.h>

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

	ImGui::Text("Position (local to parent)");

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionX", &(position.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionY", &(position.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionZ", &(position.z));
	
	ImGui::Text("Rotation (local to parent)");

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

	ImGui::Text("Scale (local to parent)");

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

void core::Transform::onGUI(const Transform& parent)
{
	float inputFieldsWidth = ImGui::GetContentRegionAvail().x * 0.25f;

	ImGui::Text("Position (local to parent)");

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionX", &(position.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionY", &(position.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	ImGui::InputFloat("##PositionZ", &(position.z));

	ImGui::Text("Rotation (local to parent)");

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

	ImGui::Text("Scale (local to parent)");

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
	// Faster with quaternions, less matrix multiplications
	Mat4 qtrnRotation = 
		(Qtrn(rotation.z, Vec3::Z) * 
			Qtrn(rotation.y, Vec3::Y) *
			Qtrn(rotation.x, Vec3::X)).toRotationMatrix();

	/*model = Mat4::translation(position) * Mat4::rotation(rotation.z, Vec3::Z) *
		Mat4::rotation(rotation.y, Vec3::Y) *
		Mat4::rotation(rotation.x, Vec3::X) *
		Mat4::scaling(scale);*/

	model = Mat4::translation(position) * qtrnRotation * Mat4::scaling(scale);

	model = parentModel * model;
}

void Transform::makeChildOfTransform(Transform* transform)
{
	// Faster with quaternions, less matrix multiplications
	/*Mat4 qtrnRotation = (Qtrn(transform->rotation.z, Vec3::Z) *
		Qtrn(transform->rotation.y, Vec3::Y) *
		Qtrn(transform->rotation.x, Vec3::X)).inverse().toRotationMatrix();*/

	/*model = Mat4::translation(position) * Mat4::rotation(rotation.z, Vec3::Z) *
		Mat4::rotation(rotation.y, Vec3::Y) *
		Mat4::rotation(rotation.x, Vec3::X) *
		Mat4::scaling(scale);*/

	//Mat4 inverseModel = Mat4::translation(-1 * transform->position) * qtrnRotation * Mat4::scaling(-1 * transform->scale);
	
	Mat4 qtrnRotation =
		(Qtrn(rotation.z, Vec3::Z) *
			Qtrn(rotation.y, Vec3::Y) *
			Qtrn(rotation.x, Vec3::X)).toRotationMatrix();

	/*model = Mat4::translation(position) * Mat4::rotation(rotation.z, Vec3::Z) *
		Mat4::rotation(rotation.y, Vec3::Y) *
		Mat4::rotation(rotation.x, Vec3::X) *
		Mat4::scaling(scale);*/

	model = Mat4::translation(position) * qtrnRotation * Mat4::scaling(scale);

	model =  transform->model * model;

	model.decompose(scale, rotation, position);
}

void Transform::removeChildOfTransform(Transform* transform)
{
	model = transform->model * model;
}

Vec3 core::Transform::getUp() const
{
	return model* Vec3(0.0f , 1.0f, 0.0f);
}

Vec3 core::Transform::getForward() const
{
	return model * Vec3(0.0f, 0.0f, 1.0f);
}

Vec3 core::Transform::getRight() const
{
	return model * Vec3(1.0f ,0.0f, 0.0f);
}
