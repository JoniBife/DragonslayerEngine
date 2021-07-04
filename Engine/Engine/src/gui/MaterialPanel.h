#ifndef MATERIAL_PANEL
#define MATERIAL_PANEL

#include "../core/Material.h"

using namespace core;

class MaterialPanel {

private:
	Material* selectedMaterial = nullptr;

public:
	MaterialPanel();

	void showMaterial(Material* material);
	void stopShowingMaterial();

	void onGUI();

};


#endif
