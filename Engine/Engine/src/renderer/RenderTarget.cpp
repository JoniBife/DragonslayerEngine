#include "RenderTarget.h"

using namespace renderer;

renderer::RenderTarget::RenderTarget()
{
	FrameBufferBuilder frameBufferBuilder;
	frameBuffer = frameBufferBuilder
		.setSize(1, 1)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();
}

renderer::RenderTarget::~RenderTarget()
{
	delete frameBuffer;
}

RenderTexture renderer::RenderTarget::getRenderTexture()
{
	RenderTexture renderTexture;
	// The render target frame buffer has a single color attachment
	// which will always be at index 0
	renderTexture.textureId = frameBuffer->getColorAttachment(0).getId();
	return renderTexture;
}
