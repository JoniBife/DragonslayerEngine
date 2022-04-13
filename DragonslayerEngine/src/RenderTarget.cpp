#include "DragonslayerEngine/RenderTarget.h"

using namespace DragonslayerEngine;

DragonslayerEngine::RenderTarget::RenderTarget()
{
	FrameBufferBuilder frameBufferBuilder;
	frameBuffer = frameBufferBuilder
		.setSize(1, 1)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();
}

DragonslayerEngine::RenderTarget::~RenderTarget()
{

}

RenderTexture DragonslayerEngine::RenderTarget::getRenderTexture()
{
	RenderTexture renderTexture;
	// The render target frame buffer has a single color attachment
	// which will always be at index 0
	renderTexture.textureId = frameBuffer.getColorAttachment(0).getId();
	return renderTexture;
}
