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

void Camera::setCameraPosition(const Vec3& position)
{
	dirty = true;
	this->position = position;
}

void Camera::setCameraTarget(const Vec3& target)
{
	dirty = true;
	this->target = target;
}

void Camera::setCameraUp(const Vec3& up)
{
	dirty = true;
	this->up = up;
}

void Camera::setNearPlane(float nearPlane)
{
	assert(nearPlane > 0.01f);
	assert(nearPlane < far);
	dirty = true;
	this->near = nearPlane;
}

void Camera::setFarPlane(float farPlane)
{
	assert(farPlane > 0);
	assert(farPlane > near);
	dirty = true;
	this->far = farPlane;
}

void Camera::setFov(float fov)
{
	// TODO Check range of fov
	assert(fov > 0);
	dirty = true;
	this->fov = fov;
}

Vec3 Camera::getCameraPosition() const
{
	return position;
}

Vec3 Camera::getCameraTarget() const
{
	return target;
}

Vec3 Camera::getCameraUp() const
{
	return up;
}

float Camera::getNearPlane() const
{
	return near;
}

float Camera::getFarPlane() const
{
	return far;
}

float Camera::getFov() const
{
	return fov;
}

Mat4 Camera::getView() const
{
	return view;
}

Mat4 Camera::getProjection() const
{
	return projection;
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
	this->cameraController->setOnMovementListener([&](Mat4& view) {
		// Each time the camera moves, we update the view matrix with the new view matrix and the projection matrix with the new projection	
		setView(view);
		setProjection(projection);
	});
}

// UboBp Getter
GLuint Camera::getUboBindingPoint() { return uboBp; }