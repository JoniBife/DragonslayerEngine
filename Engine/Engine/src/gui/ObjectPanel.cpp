#include "ObjectPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../core/Component.h"
#include "../core/MeshRenderer.h"

#include "GUI.h"

ObjectPanel::ObjectPanel()
{
}

void ObjectPanel::showObject(GameObject* gameObject)
{
	this->gameObject = gameObject;
}

void ObjectPanel::stopShowingObject()
{
	this->gameObject = nullptr;
}

void ObjectPanel::onGUI(GUI& gui)
{
	ImGui::Begin("Object");

	if (gameObject != nullptr) {
		ImGui::Text(gameObject->getName().c_str());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::CollapsingHeader("Transform", flags))
		{
			GameObject* parent = gameObject->getParent();
			if (parent) {
				gameObject->getTransform()->onGUI(*parent->getTransform());
			} else {
				gameObject->getTransform()->onGUI();
			}
		}
		ImGui::Dummy(ImVec2(0.0f, spacingBetweenComponents));

		bool enableCloseButton = true;

		for (Component* component : gameObject->getAttachedComponents()) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader(component->getName().c_str(), &enableCloseButton, flags))
			{
				component->onGUI(gui);
			}

			if (!enableCloseButton) {
				// TODO Not working for newly added duplicate component
				gameObject->removeComponent(component);
				enableCloseButton = true;
			}

			ImGui::Dummy(ImVec2(0.0f, spacingBetweenComponents));
		}

		if (ImGui::BeginPopupContextWindow())
		{
			
			if (ImGui::BeginMenu("Add component"))
			{
				if (ImGui::MenuItem("MeshRenderer")) {
					gameObject->addComponent(new MeshRenderer());
				}

				
				ImGui::EndMenu();
			}
			
			if (ImGui::MenuItem("Close")) {}
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}
