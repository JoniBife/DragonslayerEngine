#include "CubeController.h"
#include "../core/Input.h"

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

	if (Input::isKeyDown(KeyCode::A)) {
		getGameObject()->getTransform()->position.x += 0.05f;
	}

	if (Input::isMouseButtonDown(MouseButtonCode::LEFT)) {
		getGameObject()->getTransform()->scale.x += 0.005f;
	}

	if (Input::isMouseButtonUp(MouseButtonCode::LEFT)) {
		getGameObject()->getTransform()->scale.x = 1.0f;
	}
}

void CubeController::onEnd()
{
}

void CubeController::onGUI()
{
}
