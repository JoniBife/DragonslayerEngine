
#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include <math/Vec2.h>
#include <textures/Texture2D.h>
#include <textures/FrameBuffer.h>
#include <view/Camera.h>

class EditorCamera : public Camera {

private:
	Texture2D* frameTexture = nullptr;
	FrameBuffer* frameBuffer = nullptr; // TODO Swap to render target
	GLuint rbo = 0;
	float movementSpeed = 6.0f; // In units (meters) per second
	float rotationSpeed = 30.0f; // In degrees per second
	float dragSpeed = 6.0f;
	float zoomSpeed = 60.0f;
	float pitch = 0.0f;
	float yaw = -90.0f;
	bool editorWindowFocused = false; // Indicates whether the editor window is selected or not

	Vec2 lastMousePosition = { 0.0f , 0.0f };
	Vec2 startingDragPosition = { 0.0f , 0.0f };
	bool freeModeEnabled = false;
	bool dragModeEnabled = false;

	// Editor camera actions (return true if there was action) 
	bool freeMovement(float elapsedTime, Vec2 currMousePosition);
	bool drag(float elapsedTime, Vec2 currMousePosition);
	bool zoom(float elapsedTime, float scroll);

public:
	EditorCamera();
	~EditorCamera();

	void setEditorWindowFocus(bool editorWindowFocused);
	void setMovementSpeed(float movementSpeed);
	void setRotationSpeed(float rotationSpeed);
	void setDragSpeed(float dragSpeed);
	void setZoomSpeed(float zoomSpeed);

	float getMovementSpeed() const;
	float getRotationSpeed() const;
	float getDragSpeed() const;
	float getZoomSpeed() const;

	void update(float elapsedTime) override;

	Texture2D& getFrameTexture() const;
	FrameBuffer& getFrameBuffer() const;

	void onGUI();

};

#endif