#include "Camera.h"
#include "../utils/OpenGLUtils.h"
#include "Transformations.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../math/MathAux.h"

Camera::Camera() {

}

Camera::~Camera()
{
}

void Camera::update(float elapsedTime) {

	if (dirty) {
		dirty = false;
		wasDirtyRecently = true;
		//front = target - position;
		view = lookAt(position, position + front, up);
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

/*
void Camera::addCameraController(ICameraController* cameraController) {
	this->cameraController = cameraController;
	this->cameraController->setOnMovementListener([&](Mat4& view) {
		// Each time the camera moves, we update the view matrix with the new view matrix and the projection matrix with the new projection	
		setView(view);
		setProjection(projection);
	});
}*/

void Camera::onGUI()
{
	/*
	Vec3 getCameraPosition() const;
	Vec3 getCameraTarget() const;
	Vec3 getCameraUp() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getFov() const;
	Mat4 getView() const;
	Mat4 getProjection() const;
	float getViewportWidth() const;
	float getViewportHeight() const;
	Vec2 getViewportSize() const;
	*/

	float editedNear = near;
	float editedFar = far;

	ImGui::InputFloat("Near plane", &editedNear);
	ImGui::InputFloat("Far plane", &editedFar);

	SWAP_IF_DIFFERENT(near, editedNear, dirty)
	SWAP_IF_DIFFERENT(far, editedFar, dirty)

	float min = 0.01f;
	float max = 179.0f; 
	float editedFov = fov;
	ImGui::SliderScalar("Fov", ImGuiDataType_Float, &editedFov, &min, &max);

	SWAP_IF_DIFFERENT(fov, editedFov, dirty)
	

}
