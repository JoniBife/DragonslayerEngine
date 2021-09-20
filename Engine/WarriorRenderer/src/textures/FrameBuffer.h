#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "Texture2D.h"
#include <vector>
#include <functional>
#include "../GLObject.h"

class FrameBufferBuilder;

class FrameBuffer : public GLObject {

private:
	GLuint id = 0u;

	unsigned int width = 0u, height = 0u;

	// Attachments that can be sampled from (i.e. they are textures)
	std::vector<Texture2D*> colorAttachments;
	Texture2D* stencilDepthAttachment = nullptr;
	Texture2D* depthAttachment = nullptr;

	// Attachments that cannot be sampled from (i.e. they are render buffer objects)
	std::vector<GLuint> colorAttachmentsRBO;
	GLuint stencilDepthAttachmentRBO = 0u; bool attachedStencilDepth = false;
	GLuint depthAttachmentRBO = 0u; bool attachedDepth = false;

	void _deleteObject() override;

public:
	FrameBuffer(const std::function<void()>& setAttachments);
	FrameBuffer();
	~FrameBuffer();

	/* 
	* Resizes if all the attachments are textures, otherwise does not resize
	*/
	void resize(unsigned int width, unsigned int height);

	void drawBuffers();

	Texture2D& getColorAttachment(unsigned int idx) const;
	Texture2D& getStencilDepthAttachment() const;
	Texture2D& getDepthAttachment() const;
	
	GLuint getColotAttachmentRBO(unsigned int idx) const;
	GLuint getStencilDepthAttachmentRBO() const;
	GLuint getDepthAttachmentRBO() const;

	float getWidth() const;
	float getHeigth() const;

	/* Copies the pixels from one framebuffer onto another */
	static void copyPixels(FrameBuffer& source, FrameBuffer& destination, 
		GLenum bufferType = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST);

	/* Copies the pixels from one framebuffer to the default framebuffer */
	static void copyPixels(FrameBuffer& source, unsigned int width, unsigned int height,
		GLenum bufferType = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST);

	void bind(GLenum target = GL_FRAMEBUFFER);

	void unbind(GLenum target = GL_FRAMEBUFFER);

	friend class FrameBufferBuilder;
};

class FrameBufferBuilder {

#define MAX_COLOR_ATTACHMENTS 8 // Using 8 because that is the minimum max amount of color attachments that the openGL specification requires

private:
	unsigned int width = 1u, height = 1u;
	unsigned int numberOfColorAttachments = 0u;
	std::vector<bool> allowSampleColor;
	std::vector<GLenum> colorAttachmentsPrecision;
	std::vector<GLenum> colorAttachmentsFormat;
	bool hasStencilBuffer = false; bool allowSampleStencil = true;
	bool hasDepthBuffer = false; bool allowSampleDepth = true;

	void reset();

public:
	FrameBufferBuilder();

	FrameBufferBuilder& setSize(unsigned width, unsigned height);
	FrameBufferBuilder& attachColorBuffers(unsigned int number, GLenum precision, GLenum format = GL_RGBA, bool allowSample = true);
	FrameBufferBuilder& attachStencilBuffer(bool allowSample = true);
	FrameBufferBuilder& attachDepthBuffer(bool allowSample = true);
	FrameBuffer build();
};

#endif
