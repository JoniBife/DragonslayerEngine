#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "../textures/FrameBuffer.h"
#include "../textures/Texture2D.h"
#include "../math/Vec2.h"

class EditorCamera : public Camera {

private:
	Texture2D* frameTexture = nullptr;
	FrameBuffer* frameBuffer = nullptr;
	GLuint rbo = 0;
	float movementSpeed = 3.0f; // In units (meters) per second
	float rotationSpeed = 20.0f; // In degrees per second
	float dragSpeed = 2.0f;
	float zoomSpeed = 700.0f;
	float pitch = 0.0f;
	float yaw = -90.0f;
	Vec2 lastMousePosition = { 0.0f , 0.0f };

public:
	EditorCamera();
	~EditorCamera();

	void setMovementSpeed(float movementSpeed);
	void setRotationSpeed(float rotationSpeed);

	float getMovementSpeed() const;
	float getRotationSpeed() const;

	void update(float elapsedTime) override;

	Texture2D& getFrameTexture() const;
	FrameBuffer& getFrameBuffer() const;

};

#endif