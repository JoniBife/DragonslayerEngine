#ifndef ORBIT_CAMERA_CONTROLLER_3D_H
#define ORBIT_CAMERA_CONTROLLER_3D_H

#include <GLFW/glfw3.h>
#include "ICameraController.h"
#include "../math/Qtrn.h"
#include "../math/Vec3.h"

class SphericalCameraController : public ICameraController {

public:

	Vec3 eulerAngles;
	Qtrn qtrn;
	GLFWwindow* win;
	float sensitivity;
	std::function<void(Mat4&, Mat4&)> onMovement;
	float zoom;
	Vec3 initialRotation;
	Qtrn initialQtrn;

	SphericalCameraController(const Vec3& eulerAngles,const Qtrn& qtrn, GLFWwindow* win, const float initialZoom, const float sensitivity = 0.01f);

	void setOnMovementListener(const std::function<void(Mat4&, Mat4&)>& onMovement) override;

	void processInputAndMove(const float elapsedTime) override;

private:
	void processMouseInput();
	void processScrollWheelInput();
};

#endif
