#include "HierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

void HierarchyPanel::drawHierarchyRecursively(std::list<GameObject*> gameObjects)
{
	auto treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	for (GameObject* gameObject : gameObjects) {

		if (gameObject == selected)
			treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

		if (gameObject->numberOfChildren() == 0) {

			treeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
			bool open = ImGui::TreeNodeEx(gameObject->getName().c_str(), treeNodeFlags);

			if (ImGui::IsItemClicked()) {
				if (gameObject == selected) {
					selected->unselect();
					selected = nullptr;
					objectPanel.stopShowingObject();
				}
				else
				{
					if (selected != nullptr) {
						selected->unselect();
					}
					selected = gameObject;
					gameObject->select();
					objectPanel.showObject(gameObject);
				}
			}

			if (open)
				ImGui::TreePop();
		}
		else {

			bool open = ImGui::TreeNodeEx(gameObject->getName().c_str(), treeNodeFlags);

			if (ImGui::IsItemClicked()) {
				if (gameObject == selected) {
					selected->unselect();
					selected = nullptr;
					objectPanel.stopShowingObject();
				}
				else
				{
					if (selected != nullptr) {
						selected->unselect();
					}
					selected = gameObject;
					gameObject->select();
					objectPanel.showObject(gameObject);
				}
			}

			if (open) {
				drawHierarchyRecursively(gameObject->getChildren());
				ImGui::TreePop();
			}
		}

		treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	}
}

HierarchyPanel::HierarchyPanel(Hierarchy& hierarchy, ObjectPanel& objectPanel) : hierarchy(hierarchy), objectPanel(objectPanel)
{
}

GameObject* HierarchyPanel::getSelectedGameObject() const
{
	return selected;
}

void HierarchyPanel::onGUI()
{
	ImGui::Begin("Hierarchy");

	Hierarchy& hierarchy = Hierarchy::getHierarchy();

	auto rootGameObjects = hierarchy.getRootGameObjects();

	drawHierarchyRecursively(rootGameObjects);

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create GameObject")) {
			hierarchy.createGameObject();
		}
		if (ImGui::MenuItem("Close")) {}
		ImGui::EndPopup();
	}

	ImGui::End();
}
