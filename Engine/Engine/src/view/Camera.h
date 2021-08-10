#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glew.h"
#include "../math/Mat4.h"
#include "../math/Vec2.h"
#include "../controllers/ICameraController.h"
#include <assert.h>

class Camera {

protected:
	Mat4 view;
	Mat4 projection; // TODO Create a projection class
	GLuint vbo;
	GLuint uboBp = 0;
	ICameraController* cameraController;

	Vec3 position = { 0.0f, 0.0f, 3.0f }; // eye
	Vec3 target = { 0.0f, 0.0f, 0.0f }; // center
	Vec3 front = target - position;
	Vec3 up = { 0.0f, 1.0f, 0.0f }; // up

	float viewportWidth = 1366, viewportHeight = 720;

	float near = 0.01f;
	float far = 100.0f;

	float fov = 60.0f;

	bool dirty = true; // Indicates whether a setter was called

public:
	Camera();
	virtual ~Camera();

	virtual void update(float elapsedTime);

	void setPosition(const Vec3& position);
	void setTarget(const Vec3& target);
	void setUp(const Vec3& up);
	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);
	void setFov(float fov);
	void setViewportSize(float viewportWidth, float viewportHeight);

	Vec3 getPosition() const;
	Vec3 getTarget() const;
	Vec3 getUp() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getFov() const;
	Mat4 getView() const;
	Mat4 getProjection() const;
	float getViewportWidth() const;
	float getViewportHeight() const;
	Vec2 getViewportSize() const;

	//void addCameraController(ICameraController* cameraController);

	GLuint getUboBindingPoint();

	void onGUI();
};

#endif
