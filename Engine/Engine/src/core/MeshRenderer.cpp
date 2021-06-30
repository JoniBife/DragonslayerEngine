#include "MeshRenderer.h"

core::MeshRenderer::MeshRenderer() : Renderer("Mesh Renderer")
{

}

void core::MeshRenderer::onGUI()
{
	ImGui::TextWrapped("Mesh: "); ImGui::SameLine();

	const char* meshName;

	if (mesh != nullptr)
		meshName = mesh->getName().c_str();
	else
		meshName = "NOT SET";

	ImGui::TextWrapped(meshName);

	ImGui::TextWrapped("Material: "); ImGui::SameLine();

	const char* materialName;

	if (material != nullptr)
		materialName = material->getName().c_str();
	else
		materialName = "NOT SET";

	ImGui::TextWrapped(materialName);
}

void core::MeshRenderer::update()
{

}
