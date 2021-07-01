#ifndef CUBE_CONTROLLER_H
#define CUBE_CONTROLLER_H

#include "../core/Controller.h"

using namespace core;

class CubeController : public Controller {

public:
	CubeController();
	~CubeController() override;

	void onStart() override;
	void onFrameUpdate() override;
	void onEnd() override;
	void onGUI() override;

};

#endif
