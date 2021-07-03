#ifndef HIERARCHY_PANEL_H
#define HIERARCHY_PANEL_H

#include "ObjectPanel.h"
#include "../core/GameObject.h"
#include "../core/Hierarchy.h"

using namespace core;

class HierarchyPanel {

private:
	ObjectPanel& objectPanel;
	Hierarchy& hierarchy;
	GameObject* selected = nullptr;

	void drawHierarchyRecursively(std::list<GameObject*> gameObjects);

public:
	HierarchyPanel(Hierarchy& hierarchy, ObjectPanel& objectPanel);

	void onGUI();
};

#endif