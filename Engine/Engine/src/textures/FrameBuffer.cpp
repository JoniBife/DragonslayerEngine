#include <functional>
#include <assert.h>
#include "FrameBuffer.h"
#include "../utils/OpenGLUtils.h"

FrameBuffer::FrameBuffer(const std::function<void()>& attachBuffers)
{	
	GL_CALL(glGenFramebuffers(1, &id));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

	attachBuffers();

	// Frame buffer MUST have attached buffers so we stop this in compile time
	// we have to attach at least one buffer(color, depth or stencil buffer).
	// there should be at least one color attachment.
	// all attachments should be complete as well(reserved memory).
	// each buffer should have the same number of samples.
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer()
{
	GL_CALL(glDeleteBuffers(1, &id));
}

void FrameBuffer::bind()
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void FrameBuffer::unbind()
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
