#ifndef DEBUG_PANEL
#define DEBUG_PANEL

#include <WarriorRenderer.h>

class DebugPanel {

private:
	WarriorRenderer::Renderer* renderer;

public:
	DebugPanel(WarriorRenderer::Renderer* renderer);

	void onGUI();


};

#endif 
