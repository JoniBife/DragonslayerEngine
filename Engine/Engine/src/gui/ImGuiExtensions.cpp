#include "ImGuiExtensions.h"
#include "imgui/imgui.h"

bool ImGui::InputVec2(const char* label, Vec2& v)
{
	float inputFieldsWidth = ImGui::GetContentRegionAvail().x * 0.25f;

	bool valueChanged = false;

	ImGui::Text(label);

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##X", &(v.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##Y", &(v.y)); ImGui::SameLine();

	return valueChanged;
}

bool ImGui::InputVec3(const char* label, Vec3& v)
{
	float inputFieldsWidth = ImGui::GetContentRegionAvail().x * 0.25f;

	bool valueChanged = false;

	ImGui::Text(label);

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##X", &(v.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##Y", &(v.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##Z", &(v.z));

	return valueChanged;
}

bool ImGui::InputVec4(const char* label, Vec4& v)
{
	float inputFieldsWidth = ImGui::GetContentRegionAvail().x * 0.25f;

	bool valueChanged = false;

	ImGui::Text(label);

	ImGui::TextColored(ImVec4(1, 0, 0, 1), "X"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##X", &(v.x)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 1, 0, 1), "Y"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##Y", &(v.y)); ImGui::SameLine();

	ImGui::TextColored(ImVec4(0, 0, 1, 1), "Z"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##Z", &(v.z));

	ImGui::TextColored(ImVec4(1, 1, 1, 1), "W"); ImGui::SameLine();
	ImGui::SetNextItemWidth(inputFieldsWidth);
	valueChanged |= ImGui::InputFloat("##W", &(v.w));

	return valueChanged;
}
