#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Camera.h"
#include "../textures/FrameBuffer.h"
#include "../textures/Texture2D.h"

class EditorCamera : public Camera {

private:
	Texture2D* frameTexture = nullptr;
	FrameBuffer* frameBuffer = nullptr;
	GLuint rbo = 0;

public:
	EditorCamera();
	~EditorCamera();

	Texture2D& getFrameTexture() const;
	FrameBuffer& getFrameBuffer() const;

};

#endif