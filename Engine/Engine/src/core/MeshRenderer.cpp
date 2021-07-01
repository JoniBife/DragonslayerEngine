#include "MeshRenderer.h"

using namespace core;

MeshRenderer::MeshRenderer() : Renderer("Mesh Renderer")
{

}

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : Renderer("Mesh Renderer"), mesh(mesh), material(material) {}

void MeshRenderer::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshRenderer::setMaterial(Material* material)
{
	this->material = material;
}

Mesh* core::MeshRenderer::getMesh() const
{
	return mesh;
}

Material* core::MeshRenderer::getMaterial() const
{
	return material;
}

void MeshRenderer::onGUI()
{
	ImGui::TextWrapped("Mesh: "); ImGui::SameLine();

	std::string meshName;

	if (mesh != nullptr)
		meshName = mesh->getName();
	else
		meshName = "NOT SET";

	ImGui::TextWrapped(meshName.c_str());

	ImGui::TextWrapped("Material: "); ImGui::SameLine();

	std::string materialName;

	if (material != nullptr)
		materialName = material->getName();
	else
		materialName = "NOT SET";

	ImGui::TextWrapped(materialName.c_str());
}

void MeshRenderer::update()
{

}
