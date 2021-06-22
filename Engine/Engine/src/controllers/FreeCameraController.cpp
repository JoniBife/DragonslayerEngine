#include "FreeCameraController.h"
#include "../view/Transformations.h"
#include "../Configurations.h"
#include "../math/MathAux.h"


FreeCameraController::FreeCameraController(const float movementSpeed, const Vec3& position, const Vec3& front, const Vec3& up, const float yaw, const float pitch,
	Mat4 orthoProj, Mat4 perspectiveProj, GLFWwindow* win)
	: movementSpeed(movementSpeed), position(position), front(front.normalize()), up(up), yaw(yaw), pitch(pitch),orthoProj(orthoProj), perspectiveProj(perspectiveProj), currProj(perspectiveProj), win(win) {

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	lastXpos = SCREEN_WIDTH / 2;
	lastYpos = SCREEN_HEIGHT / 2;

	glfwSetCursorPos(win, lastXpos, lastYpos);
}

void FreeCameraController::setOnMovementListener(const std::function<void(Mat4&, Mat4&)>& onMovement) {
	this->onMovement = onMovement;
}

/*FreeCameraController& FreeCameraController::operator=(const FreeCameraController& cameraController) {
	onMovement = cameraController.onMovement;
	win = cameraController.win;
	movementSpeed = cameraController.movementSpeed;
	position = cameraController.position;
	front = cameraController.front;
	up = cameraController.up;
	yaw = cameraController.yaw;
	pitch = cameraController.pitch;
	lastXpos = cameraController.lastXpos;
	lastYpos = cameraController.lastYpos;
	return *this;
}*/

/*
* Receives the user input and updates the view matrix accordingly
*/
void FreeCameraController::processInputAndMove(const float elapsedTime) {
	bool moved = processMouseInput();
	moved = processKeyboardInput(elapsedTime) || moved;
	if (moved) {
		Mat4 view = lookAt(position, position + front, up);
		onMovement(view, currProj);
	}
}

void FreeCameraController::snapToPosition(const Vec3 position, const Vec3 front, float yaw, float pitch) {

	glfwSetCursorPos(win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	this->front = front;
	this->position = position;
	lastXpos = SCREEN_WIDTH / 2;
	lastYpos = SCREEN_HEIGHT / 2;
	this->yaw = yaw;
	this->pitch = pitch;
	Mat4 view = lookAt(position, position + front, this->up);
	onMovement(view, currProj);
}

/*
* Checks for keyboard input, if there is input return true
* else returns false
*/
bool FreeCameraController::processKeyboardInput(const float elapsedTime) {

	bool moved = false;

	if (canControl) {
		// Converting from units per second to units per frameMesh
		float ms = movementSpeed * elapsedTime;

		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
			position += (ms)*front;
			moved = true;
		}
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= (ms)*front;
			moved = true;
		}
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= cross(front, up).normalize() * (ms);
			moved = true;
		}
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += cross(front, up).normalize() * (ms);
			moved = true;
		}

		static bool isReleased = false;
		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS && isReleased) {
			isReleased = false;
			currProj = (currProj == orthoProj) ? perspectiveProj : orthoProj;
			moved = true;
		}
		else if (glfwGetKey(win, GLFW_KEY_P) == GLFW_RELEASE) {
			isReleased = true;
		}
	}

	static bool isReleasedESC = false;
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS && isReleasedESC) {
		isReleasedESC = false;
		canControl = !canControl;

		// We ensure that the mouse returns to where it was after ESC is pressed
		if (!canControl) {
			glfwGetCursorPos(win, &XPosBeforeESC, &YPosBeforeESC);
			glfwSetCursorPos(win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
		else {
			glfwSetCursorPos(win, XPosBeforeESC, YPosBeforeESC);
		}

		glfwSetInputMode(win, GLFW_CURSOR, canControl ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
	else if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
		isReleasedESC = true;
	}
	
	return moved;
}

bool firstMove = true;


/*
* Checks for mouse input, if there is input return true
* else returns false
*/
bool FreeCameraController::processMouseInput() {

	if (canControl) {
		// Getting mouse position
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);

		double mouseSensitivity = 0.05;

		double xOffset = xpos - lastXpos;
		double yOffset = lastYpos - ypos; // Y axis is inverted (increases downward)
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		lastXpos = xpos;
		lastYpos = ypos;

		if (xOffset == 0.0 && yOffset == 0.0) {
			return false;
		}

		yaw += float(xOffset);
		pitch += float(yOffset);

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		Vec3 direction;
		direction.x = cosf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
		direction.y = sinf(degreesToRadians(pitch));
		direction.z = sinf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
		front = direction.normalize();

		return true;
	}
	return false;
}