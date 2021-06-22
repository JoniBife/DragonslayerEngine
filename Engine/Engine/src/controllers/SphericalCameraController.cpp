#include "SphericalCameraController.h"
#include "../math/Vec2.h"

bool quatMode = true;
double yOffset = 0;

void glfw_key_G_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		quatMode = quatMode ? false : true;
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	yOffset = yoffset;
}

SphericalCameraController::SphericalCameraController(const Vec3& eulerAngles, const Qtrn& qtrn, GLFWwindow* win, const float initialZoom, const float sensitivity)
: eulerAngles(eulerAngles), qtrn(qtrn), win(win), sensitivity(sensitivity), zoom(initialZoom) {

	initialRotation = eulerAngles;
	initialQtrn = qtrn;

	glfwSetKeyCallback(win, glfw_key_G_callback);
	glfwSetScrollCallback(win, scroll_callback);
}

void SphericalCameraController::setOnMovementListener(const std::function<void(Mat4&, Mat4&)>& onMovement) {
	this->onMovement = onMovement;
}

void SphericalCameraController::processInputAndMove(const float elapsedTime) {
	processScrollWheelInput();
	processMouseInput();

	Mat4 viewMatrix;
	if (quatMode) {
		if (eulerAngles != initialRotation) eulerAngles = initialRotation;
		viewMatrix = Mat4::translation(0.0f, 0.0f, zoom) * qtrn.toRotationMatrix();
	}
	else {
		if (qtrn != initialQtrn) qtrn = initialQtrn;
		viewMatrix = Mat4::translation(0.0f, 0.0f, zoom) * Mat4::rotation(eulerAngles.y, Vec3::X) * Mat4::rotation(eulerAngles.x, Vec3::Y);
		
	}
	onMovement(viewMatrix, Mat4::IDENTITY);
}

void SphericalCameraController::processMouseInput() {
	static Vec2 lastPos;
	static bool isPressed = false;

	int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);
	if (state == GLFW_PRESS)
	{
		if (isPressed) {

			Vec2 currPos((float)xpos, (float)ypos);

			// Direction of mouse movement
			Vec2 dir = (currPos - lastPos);

			if (dir != Vec2(0, 0)) {
				dir = dir * sensitivity;

				if (quatMode) {	
					qtrn = (Qtrn(dir.x, Vec3::Y) * Qtrn(dir.y, Vec3::X)).normalize() * qtrn;
				}
				else {
					eulerAngles.x += dir.x;
					eulerAngles.y += dir.y;
				}
			}
		}
		else {
			isPressed = true;
		}
		lastPos = Vec2((float)xpos, (float)ypos);
	}
	else if (state == GLFW_RELEASE) {
		isPressed = false;
	}
}

void SphericalCameraController::processScrollWheelInput() {
	// This is not limiting the zoom, it is possible to continue zooming out indefinetly
	float scrollWheelSensitivity = 0.1f;
	zoom += float(yOffset) * scrollWheelSensitivity;

	if (zoom >= -0.1f)
		zoom = -0.1f;
	else if (zoom <= -100.0f)
		zoom = -100.0f;

	yOffset = 0.0;
}