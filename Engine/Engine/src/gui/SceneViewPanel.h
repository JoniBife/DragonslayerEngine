#ifndef SCENE_VIEW_PANEL_H
#define SCENE_VIEW_PANEL_H

#include "../view/EditorCamera.h"
#include "HierarchyPanel.h"
#include <imGuizmo/ImGuizmo.h>

class SceneViewPanel {

private:
	EditorCamera& editorCamera;
	HierarchyPanel& hierarchyPanel;
	ImGuizmo::OPERATION selectedOperation = ImGuizmo::OPERATION::TRANSLATE;
	
public:
	SceneViewPanel(EditorCamera& camera, HierarchyPanel& hierarchyPanel);

	void onGUI();

};

#endif
