#include <functional>
#include <assert.h>
#include "FrameBuffer.h"
#include "../utils/OpenGLUtils.h"
#include "../Configurations.h"

FrameBuffer::FrameBuffer()
{
}

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

	for (Texture2D* texture : colorAttachments) {
		delete texture;
	}

	if (stencilDepthAttachment != nullptr)
		delete stencilDepthAttachment;
	else if (depthAttachment != nullptr)
		delete depthAttachment;

	// vectors stores its elements in contiguous memory so it's safe to pass a pointer to the first element
	if (colorAttachmentsRBO.size() > 0)
		GL_CALL(glDeleteRenderbuffers(colorAttachmentsRBO.size(), &colorAttachmentsRBO[0]));

	if (attachedStencilDepth) 
		GL_CALL(glDeleteRenderbuffers(1, &stencilDepthAttachmentRBO));
	else if (attachedDepth) 
		GL_CALL(glDeleteRenderbuffers(1, &depthAttachmentRBO));

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

void FrameBuffer::drawBuffers()
{
	assert(colorAttachments.size() > 0 || colorAttachmentsRBO.size() > 0);

	std::vector<GLuint> attachments;
	if (colorAttachments.size() > 0) {
		for (int i = GL_COLOR_ATTACHMENT0; i < GL_COLOR_ATTACHMENT0 + colorAttachments.size(); ++i) {
			attachments.push_back(i);
		}
	} else if (colorAttachmentsRBO.size() > 0) {
		for (int i = GL_COLOR_ATTACHMENT0; i < GL_COLOR_ATTACHMENT0 + colorAttachmentsRBO.size(); ++i) {
			attachments.push_back(i);
		}
	}

	// Vectors store its elements in contiguous memory so its safe to pass a pointer to the first element
	GL_CALL(glDrawBuffers(attachments.size(), &attachments[0]));
}

Texture2D& FrameBuffer::getColorAttachment(unsigned int idx) const
{
	assert(colorAttachments.size() > 0 &&  idx < colorAttachments.size());
	return *colorAttachments[idx];
}

Texture2D& FrameBuffer::getStencilDepthAttachment() const
{
	assert(stencilDepthAttachment != nullptr);
	return *stencilDepthAttachment;
}

Texture2D& FrameBuffer::getDepthAttachment() const
{
	assert(depthAttachment != nullptr);
	return *depthAttachment;
}

GLuint FrameBuffer::getColotAttachmentRBO(unsigned int idx) const
{
	assert(colorAttachmentsRBO.size() > 0 && idx < colorAttachmentsRBO.size());
	return colorAttachmentsRBO[idx];
}

GLuint FrameBuffer::getStencilDepthAttachmentRBO() const
{
	assert(attachedStencilDepth);
	return stencilDepthAttachmentRBO;
}

GLuint FrameBuffer::getDepthAttachmentRBO() const
{
	assert(attachedDepth);
	return depthAttachmentRBO;
}

float FrameBuffer::getWidth() const
{
	return width;
}

float FrameBuffer::getHeigth() const
{
	return height;
}

void FrameBuffer::bind(GLenum target)
{
	GL_CALL(glBindFramebuffer(target, id));
}

void FrameBuffer::unbind(GLenum target)
{
	GL_CALL(glBindFramebuffer(target, 0));
}

void FrameBufferBuilder::reset()
{
	width = 1; height = 1;
	numberOfColorAttachments = 0;
	allowSampleColor.clear();
	colorAttachmentsPrecision.clear();
	colorAttachmentsFormat.clear();
	hasStencilBuffer = false; allowSampleStencil = true;
	hasDepthBuffer = false; allowSampleDepth = true;
}

FrameBufferBuilder::FrameBufferBuilder()
{
}

FrameBufferBuilder& FrameBufferBuilder::setSize(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
	return *this;
}

FrameBufferBuilder& FrameBufferBuilder::attachColorBuffers(unsigned int number, GLenum precision, GLenum format, bool allowSample)
{
	assert(number <= MAX_COLOR_ATTACHMENTS);
	assert(numberOfColorAttachments + number <= MAX_COLOR_ATTACHMENTS);

#ifdef DEBUG
	// Color attachments have to all be of the same type
	for (int i = 0; i < numberOfColorAttachments; ++i) {
		assert(colorAttachmentsPrecision[i] == precision);
	}
#endif

	this->numberOfColorAttachments += number;
	for (int i = 0; i < number; ++i) {
		colorAttachmentsFormat.push_back(format);
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
	
	frameBuffer->bind();


	// 1. First do all color attachments (if there are any)
	for (int i = 0; i < numberOfColorAttachments; ++i) {

		// If the allow sample if enabled then we attach a texture to the framebuffer
		// otherwise we attach a render buffer object
		if (allowSampleColor[i]) {
			
			// TODO Is this the right approach, it does not
			// feel intuitive no flexible

			GLenum format = colorAttachmentsFormat[i];
			GLenum precision = colorAttachmentsPrecision[i];
			GLint internalFormat;
			switch (format) {

				case GL_RED: {
					if (precision == GL_FLOAT) {
						internalFormat = GL_R32F;
					}
					else if (precision == GL_HALF_FLOAT) {
						internalFormat = GL_R16F;
					}
					else {
						internalFormat = GL_RED;
					}
					break;
				}
				case GL_RG: {
					if (precision == GL_FLOAT) {
						internalFormat = GL_RG32F;
					}
					else if (precision == GL_HALF_FLOAT) {
						internalFormat = GL_RG16F;
					}
					else {
						internalFormat = GL_RG;
					}
					break;
				}
				case GL_RGB: {
					if (precision == GL_FLOAT) {
						internalFormat = GL_RGB32F;
					}
					else if (precision == GL_HALF_FLOAT) {
						internalFormat = GL_RGB16F;
					}
					else {
						internalFormat = GL_RGB16F;
					}
					break;
				}
				case GL_RGBA: {
					if (precision == GL_FLOAT) {
						internalFormat = GL_RGBA32F;
					}
					else if (precision == GL_HALF_FLOAT) {
						internalFormat = GL_RGBA16F;
					}
					else {
						internalFormat = GL_RGBA;
					}
					break;
				}
				default: {
					format = GL_RGBA;
					internalFormat = GL_RGBA;
					break;
				}
			}

			Texture2D* texture = Texture2D::emptyTexture(
				width,
				height,
				internalFormat,
				format,
				precision);

			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getId(), 0));
			frameBuffer->colorAttachments.push_back(texture);

		} else {
			// TODO Is this the right approach, it does not
			// feel intuitive nor flexible
			GLenum format = colorAttachmentsFormat[i];
			GLenum precision = colorAttachmentsPrecision[i];
			GLint internalFormat;
			switch (format) {

			case GL_RED: {
				if (precision == GL_FLOAT) {
					internalFormat = GL_R32F;
				}
				else if (precision == GL_HALF_FLOAT) {
					internalFormat = GL_R16F;
				}
				else {
					internalFormat = GL_R;
				}
				break;
			}
			case GL_RG: {
				if (precision == GL_FLOAT) {
					internalFormat = GL_RG32F;
				}
				else if (precision == GL_HALF_FLOAT) {
					internalFormat = GL_RG16F;
				}
				else {
					internalFormat = GL_RG;
				}
				break;
			}
			case GL_RGB: {
				if (precision == GL_FLOAT) {
					internalFormat = GL_RGB32F;
				}
				else if (precision == GL_HALF_FLOAT) {
					internalFormat = GL_RGB16F;
				}
				else {
					internalFormat = GL_RGB16F;
				}
				break;
			}
			case GL_RGBA: {
				if (precision == GL_FLOAT) {
					internalFormat = GL_RGBA32F;
				}
				else if (precision == GL_HALF_FLOAT) {
					internalFormat = GL_RGBA16F;
				}
				else {
					internalFormat = GL_RGBA;
				}
				break;
			}
			default: {
				format = GL_RGBA;
				internalFormat = GL_RGBA;
				break;
			}
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

		frameBuffer->attachedStencilDepth = true;
	}
	else if (hasDepthBuffer) {
		if (allowSampleDepth) {
			Texture2D* texture =Texture2D::depthTexture(width, height);

			GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0));
			frameBuffer->depthAttachment = texture;

			if (numberOfColorAttachments == 0) {
				GL_CALL(glDrawBuffer(GL_NONE));
				GL_CALL(glReadBuffer(GL_NONE));
			}
		}
		else {
			GL_CALL(glGenRenderbuffers(1, &frameBuffer->depthAttachmentRBO));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->depthAttachmentRBO));
			GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
			GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		}

		frameBuffer->attachedDepth = true;
	}

	GL_CALL(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	// 3. Finally reset the FrameBufferBuilder for any future builds
	reset();

	return frameBuffer;
}
