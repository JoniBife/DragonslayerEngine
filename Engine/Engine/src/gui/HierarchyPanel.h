#ifndef HIERARCHY_PANEL_H
#define HIERARCHY_PANEL_H

#include "../core/GameObject.h"
#include "../core/Hierarchy.h"

class HierarchyPanel {

private:
	GameObject* selected = nullptr;
	Hierarchy* hierarchy = nullptr;

public:

	void OnGUI();

	friend class GUI;
};

#endif
