#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glew.h"
#include "../math/Mat4.h"
#include "../controllers/ICameraController.h"
#include <assert.h>

/*
* Usage example:
* 
* Camera camera(view, projection);
* 
* // Maybe add a camera controller
* camera.addCameraController(cameraController);
* 
* // Maybe update view
* camera.setView(newView);
* 
* // Finally update the camera
* camera.update();
*/
class Camera {

private:
	Mat4 view;
	Mat4 projection;
	GLuint vbo;
	GLuint uboBp;
	ICameraController* cameraController;

	Vec3 position = { 0.0f, 0.0f, 5.0f }; // eye
	Vec3 target = { 0.0f, 0.0f, 0.0f }; // center
	Vec3 front = target - position;
	Vec3 up = { 0.0f, 1.0f, 0.0f }; // up

	float near = 0.01f;
	float far = 100.0f;

	float fov = 45.0f;

	bool dirty; // Indicates whether a setter was called

public:
	Camera(const Mat4& view, const Mat4& projection, const GLuint uboBp);

	void update(float elapsedTime);

	void setCameraPosition(const Vec3& position);
	void setCameraTarget(const Vec3& target);
	void setCameraUp(const Vec3& up);
	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);
	void setFov(float fov);

	Vec3 getCameraPosition() const;
	Vec3 getCameraTarget() const;
	Vec3 getCameraUp() const;
	float getNearPlane() const;
	float getFarPlane() const;
	float getFov() const;
	Mat4 getView() const;
	Mat4 getProjection() const;

	void addCameraController(ICameraController* cameraController);

	GLuint getUboBindingPoint();
};

#endif
