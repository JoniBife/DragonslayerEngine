#include "Camera.h"
#include "../utils/OpenGLUtils.h"

Camera::Camera(const Mat4& view, const Mat4& projection, const GLuint uboBp) : view(view), projection(projection) {
	GL_CALL(glGenBuffers(1, &vbo));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, vbo));
	{
		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 2, 0, GL_DYNAMIC_DRAW));
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, uboBp, vbo));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void Camera::update(float elapsedTime) {

	// Update camera controller if there is one
	if (cameraController)
		cameraController->processInputAndMove(elapsedTime);

	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, vbo));
	{
		float viewOpenGLFormat[16];
		float projectionOpenGLFormat[16];
		view.toOpenGLFormat(viewOpenGLFormat);
		projection.toOpenGLFormat(projectionOpenGLFormat);
		GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(viewOpenGLFormat), viewOpenGLFormat));
		GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(viewOpenGLFormat), sizeof(projectionOpenGLFormat), projectionOpenGLFormat));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

// Sets the view matrix
void Camera::setView(const Mat4& view) {
	this->view = view;
}

// Sets the projection matrix
void Camera::setProjection(const Mat4& projection) {
	this->projection = projection;
}

// Adds the FreeCameraController
void Camera::addCameraController(ICameraController* cameraController) {
	this->cameraController = cameraController;
	this->cameraController->setOnMovementListener([&](Mat4& view, Mat4& proj) {
		// Each time the camera moves, we update the view matrix with the new view matrix and the projection matrix with the new projection	
		setView(view);
		if (proj != Mat4::IDENTITY)
			setProjection(proj);
	});
}

// UboBp Getter
GLuint Camera::getUboBindingPoint() { return uboBp; }