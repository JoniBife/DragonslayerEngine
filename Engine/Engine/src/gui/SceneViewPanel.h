#ifndef SCENE_VIEW_PANEL_H
#define SCENE_VIEW_PANEL_H

#include "../view/Camera.h"
#include "HierarchyPanel.h"

class SceneViewPanel {

private:
	Camera& editorCamera;
	HierarchyPanel& hierarchyPanel;

public:
	SceneViewPanel(Camera& camera, HierarchyPanel& hierarchyPanel);

	void onGUI();

};

#endif
