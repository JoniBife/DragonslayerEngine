#include "MaterialPanel.h"
#include <imgui/imgui.h>

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

		float roughness = selectedMaterial->getRoughness();
		ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
		selectedMaterial->setRoughness(roughness);

		float metallic = selectedMaterial->getMetallic();
		ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
		selectedMaterial->setMetallic(metallic);

		float ao = selectedMaterial->getAO();
		ImGui::SliderFloat("AO", &ao, 0.0f, 1.0f);
		selectedMaterial->setAO(ao);

		Vec3 albedoTint = selectedMaterial->getAlbedoTint();
		float color[3];
		albedoTint.toOpenGLFormat(color);
		ImGui::ColorPicker3("Albedo Tint", color);
		selectedMaterial->setAlbedoTint({ color[0], color[1], color[2] });
	}

	ImGui::End();
}
