#include "ObjectPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../core/Component.h"

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

void ObjectPanel::onGUI()
{
	ImGui::Begin("Object");

	if (gameObject != nullptr) {
		ImGui::Text(gameObject->getName().c_str());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::CollapsingHeader("Transform", flags))
		{
			gameObject->getTransform()->onGUI();
		}
		ImGui::Dummy(ImVec2(0.0f, 2.5f));


		for (Component* component : gameObject->getAttachedComponents()) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader(component->getName().c_str(), flags))
			{
				component->onGUI();
			}
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
		}
	}

	ImGui::End();
}
