#include "MeshRenderer.h"

core::MeshRenderer::MeshRenderer() : Renderer("Mesh Renderer")
{

}

core::MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) : Renderer("Mesh Renderer"), mesh(mesh), material(material) {}

void core::MeshRenderer::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void core::MeshRenderer::setMaterial(Material* material)
{
	this->material = material;
}

void core::MeshRenderer::onGUI()
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

void core::MeshRenderer::update()
{
	// Cannot render the mesh if the mesh or the material are not set
	if (mesh == nullptr || material == nullptr)
		return;

	material->use();
	mesh->bind();

	Transform* transform = gameObject->getTransform();
	material->sendParametersToShader(transform->getModelMatrix());
	mesh->draw();

	mesh->draw();
	material->stopUsing();
}
