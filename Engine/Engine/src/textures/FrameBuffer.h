#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

class FrameBuffer {

private:
	GLuint id;
	

public:
	GLuint rbo;
	FrameBuffer(const std::function<void()>& setAttachments);
	~FrameBuffer();

	void bind();

	void unbind();
};

#endif
