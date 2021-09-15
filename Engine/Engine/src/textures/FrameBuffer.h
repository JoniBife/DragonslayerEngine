#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include "Texture2D.h"
#include <vector>
#include <functional>

class FrameBufferBuilder;

class FrameBuffer {

private:
	GLuint id;

	unsigned int width, height;

	// Attachments that can be sampled from (i.e. they are textures)
	std::vector<Texture2D*> colorAttachments;
	Texture2D* stencilDepthAttachment = nullptr;
	Texture2D* depthAttachment = nullptr;

	// Attachments that cannot be sampled from (i.e. they are render buffer objects)
	std::vector<GLuint> colorAttachmentsRBO;
	GLuint stencilDepthAttachmentRBO; bool attachedStencilDepth = false;
	GLuint depthAttachmentRBO; bool attachedDepth = false;

	FrameBuffer();

public:

	FrameBuffer(const std::function<void()>& setAttachments);
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

	void bind(GLenum target = GL_FRAMEBUFFER);

	void unbind(GLenum target = GL_FRAMEBUFFER);

	friend class FrameBufferBuilder;
};

class FrameBufferBuilder {

#define MAX_COLOR_ATTACHMENTS 8 // Using 8 because that is the minimum max amount of color attachments that the openGL specification requires

private:
	unsigned int width = 1, height = 1;
	unsigned int numberOfColorAttachments = 0;
	std::vector<bool> allowSampleColor;
	std::vector<GLenum> colorAttachmentsPrecision;
	bool hasStencilBuffer = false; bool allowSampleStencil = true;
	bool hasDepthBuffer = false; bool allowSampleDepth = true;

	void reset();

public:
	FrameBufferBuilder();

	FrameBufferBuilder& setSize(unsigned width, unsigned height);
	FrameBufferBuilder& attachColorBuffers(unsigned int number, GLenum precision, bool allowSample = true);
	FrameBufferBuilder& attachStencilBuffer(bool allowSample = true);
	FrameBufferBuilder& attachDepthBuffer(bool allowSample = true);
	FrameBuffer* build();
};

#endif
