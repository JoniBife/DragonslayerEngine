#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Mat4.h"
#include "../math/Vec2.h"
#include <assert.h>

class Camera {

protected:
	Mat4 view;
	Mat4 projection; // TODO Create a projection class

	Vec3 position = { 0.0f, 0.0f, 10.0f }; // eye
	Vec3 target = { 0.0f, 0.0f, 0.0f }; // center
	Vec3 front = target - position;
	Vec3 up = { 0.0f, 1.0f, 0.0f }; // up
	Vec3 right = {1.0f, 0.0f, 0.0f};

	float viewportWidth = 1366, viewportHeight = 768;

	float near = 0.01f;
	float far = 100.0f;

	float fov = 60.0f;

	bool wasDirtyRecently = false; // Indicates whether the camera was updated recently
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
	Vec3 getFront() const;
	Vec3 getUp() const;
	Vec3 getRight() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getFov() const;
	Mat4 getView() const;
	Mat4 getProjection() const;
	float getViewportWidth() const;
	float getViewportHeight() const;
	Vec2 getViewportSize() const;
	bool wasDirty() const; // Checks if the camera was dirty recently
	float getAspectRatio() const;
};

#endif
