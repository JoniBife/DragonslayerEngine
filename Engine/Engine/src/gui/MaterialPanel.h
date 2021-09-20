#ifndef MATERIAL_PANEL
#define MATERIAL_PANEL

#include <Material.h>

using namespace WarriorRenderer;

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
