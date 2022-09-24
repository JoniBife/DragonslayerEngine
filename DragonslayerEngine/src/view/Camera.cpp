#include "DragonslayerEngine/Camera.h"
#include "DragonslayerEngine/utils/OpenGLUtils.h"
#include "Transformations.h"
#include <LMath/MathAux.hpp>

Camera::Camera() {

}

Camera::~Camera()
{
}

void Camera::update(float elapsedTime) {

	if (dirty) {
		dirty = false;
		wasDirtyRecently = true;
		right = cross(front, Vec3::Y).normalize();
		up = cross(right, front).normalize();

		//front = target - position;
		view = lookAt(position, position + front, up);
		//projection = ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 30.0f);
		projection = perspective(degreesToRadians(fov), viewportWidth / viewportHeight, near, far);
	} else {
		wasDirtyRecently = false;
	}
}

void Camera::setPosition(const Vec3& position)
{
	dirty = true;
	this->position = position;
}

void Camera::setTarget(const Vec3& target)
{
	dirty = true;
	this->target = target;
}

void Camera::setUp(const Vec3& up)
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

void Camera::setViewportSize(float viewportWidth, float viewportHeight)
{
	assert(viewportWidth > 0 && viewportHeight > 0);
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
}

Vec3 Camera::getPosition() const
{
	return position;
}

Vec3 Camera::getTarget() const
{
	return target;
}

Vec3 Camera::getUp() const
{
	return up;
}

Vec3 Camera::getRight() const
{
	return right;
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

float Camera::getViewportWidth() const
{
	return viewportWidth;
}

float Camera::getViewportHeight() const
{
	return viewportHeight;
}

Vec2 Camera::getViewportSize() const
{
	return Vec2(viewportWidth, viewportHeight);
}

bool Camera::wasDirty() const
{
	return wasDirtyRecently;
}

float Camera::getAspectRatio() const
{
	return viewportWidth / viewportHeight;
}

Vec3 Camera::getFront() const
{
	return front;
}