#ifndef OBJECT_PANEL_H
#define OBJECT_PANEL_H

#include "../core/GameObject.h"

using namespace core;

class ObjectPanel {

private:
	GameObject* gameObject = nullptr;
	float spacingBetweenComponents = 2.5f;

public:
	ObjectPanel();

	void showObject(GameObject* gameObject);
	void stopShowingObject();

	void onGUI();
};

#endif

