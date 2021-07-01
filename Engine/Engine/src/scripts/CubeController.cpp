#include "CubeController.h"



CubeController::CubeController() : Controller("Cube Rotator")
{
}

CubeController::~CubeController()
{
}

void CubeController::onStart()
{
}

void CubeController::onFrameUpdate()
{
	getGameObject()->getTransform()->rotation.y += 0.001f;
}

void CubeController::onEnd()
{
}

void CubeController::onGUI()
{
}
