#include <functional>
#include <assert.h>
#include "FrameBuffer.h"
#include "../utils/OpenGLUtils.h"
#include "../Configurations.h"

FrameBuffer::FrameBuffer(const std::function<void()>& attachBuffers)
{	
	GL_CALL(glGenFramebuffers(1, &id));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));

	attachBuffers();

	// Framebuffers MUST have attached buffers so we stop this in compile time
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

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
	assert(colorAttachments.size() > 0 && (stencilDepthAttachment != nullptr || depthAttachment != nullptr));

	for (Texture2D* texture : colorAttachments) {
		texture->resize(width, height);
	}

	if (stencilDepthAttachment != nullptr)
		stencilDepthAttachment->resize(width, height);

	if (depthAttachment != nullptr)
		depthAttachment->resize(width, height);
}

void FrameBuffer::bind()
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void FrameBuffer::unbind()
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBufferBuilder::reset()
{
	width = 1; height = 1;
	numberOfColorAttachments = 0;
	allowSampleColor.clear();
	colorAttachmentsPrecision.clear();
	hasStencilBuffer = false; allowSampleStencil = true;
	hasDepthBuffer = false; allowSampleDepth = true;
}

FrameBufferBuilder::FrameBufferBuilder()
{
}

FrameBufferBuilder& FrameBufferBuilder::setSize(unsigned width, unsigned height)
{
	this, width = width;
	this->height = height;
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::attachColorBuffers(unsigned int number, GLenum precision, bool allowSample)
{
	assert(number <= MAX_COLOR_ATTACHMENTS);
	assert(numberOfColorAttachments + number <= MAX_COLOR_ATTACHMENTS);

#ifdef DEBUG
	// Color attachments have to all be of the same type
	for (int i = 0; i < numberOfColorAttachments; ++i) {
		assert(colorAttachmentsPrecision[i] == allowSample);
	}
#endif

	this->numberOfColorAttachments += number;
	for (int i = 0; i < number; ++i) {
		colorAttachmentsPrecision.push_back(precision);
		allowSampleColor.push_back(allowSample);
	}

	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::attachStencilBuffer(bool allowSample)
{
	hasStencilBuffer = true;
	allowSampleStencil = allowSample;

	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::attachDepthBuffer(bool allowSample)
{
	hasDepthBuffer = true;
	allowSampleDepth = allowSample;

	return *this;
}

FrameBuffer* FrameBufferBuilder::build()
{
	// A frame buffer has to have at least one attachment
	assert(numberOfColorAttachments > 0 || hasDepthBuffer || hasStencilBuffer);

#ifdef DEBUG
	// Only from OpenGL 4.4 can there be pure stencil textures, so we are not allowing them for now
	// if a stencil buffer is to be attached then a depth buffer also has to be
	// https://stackoverflow.com/questions/37506460/opengl-framebuffer-separate-stencil-texture
	if (hasStencilBuffer) {
		assert(hasDepthBuffer);
		assert(allowSampleStencil == allowSampleDepth);
	}
#endif

	FrameBuffer* frameBuffer = new FrameBuffer();
	frameBuffer->width = width;
	frameBuffer->height = height;

	GL_CALL(glGenFramebuffers(1, &frameBuffer->id));
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->id));

	// 1. First do all color attachments (if there are any)
	for (int i = 0; i < numberOfColorAttachments; ++i) {

		// If the allow sample if enabled then we attach a texture to the framebuffer
		// otherwise we attach a render buffer object
		if (allowSampleColor[i]) {
			
			// TODO Is this the right approach, it does not
			// feel intuitive no flexible
			GLint internalFormat = GL_RGBA;
			if (colorAttachmentsPrecision[i] == GL_FLOAT) {
				internalFormat = GL_RGBA32F;
			} else if (colorAttachmentsPrecision[i] == GL_HALF_FLOAT) {
				internalFormat = GL_RGBA16F;
			}

			Texture2D* texture = Texture2D::emptyTexture(
				width,
				height,
				internalFormat,
				GL_RGBA,
				colorAttachmentsPrecision[i]);

			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getId(), 0));
			frameBuffer->colorAttachments.push_back(texture);

		} else {
			// TODO Is this the right approach, it does not
			// feel intuitive no flexible
			GLint internalFormat = GL_RGBA;
			if (colorAttachmentsPrecision[i] == GL_FLOAT) {
				internalFormat = GL_RGBA32F;
			}
			else if (colorAttachmentsPrecision[i] == GL_HALF_FLOAT) {
				internalFormat = GL_RGBA16F;
			}

			GLuint rbo;
			GL_CALL(glGenRenderbuffers(1, &rbo));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

			GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rbo));
			frameBuffer->colorAttachmentsRBO.push_back(rbo);
		}
	}

	// 2. Finally attach stencil and depth, or only depth (if they were requested)
	if (hasDepthBuffer && hasStencilBuffer) {
		if (allowSampleDepth && allowSampleStencil) {
			Texture2D* texture =
				Texture2D::emptyTexture(width, height, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0));
			frameBuffer->stencilDepthAttachment = texture;
		}
		else {
			GL_CALL(glGenRenderbuffers(1, &frameBuffer->stencilDepthAttachmentRBO));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->stencilDepthAttachmentRBO));
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		}
	}
	else if (hasDepthBuffer) {
		if (allowSampleDepth) {
			// TODO Check if this attachment type is correct
			Texture2D* texture =
				Texture2D::emptyTexture(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);

			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0));
			frameBuffer->depthAttachment = texture;
		}
		else {
			GL_CALL(glGenRenderbuffers(1, &frameBuffer->depthAttachmentRBO));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->depthAttachmentRBO));
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		}
	}

	// 3. Finally reset the FrameBufferBuilder for any future builds
	reset();

	return frameBuffer;
}
