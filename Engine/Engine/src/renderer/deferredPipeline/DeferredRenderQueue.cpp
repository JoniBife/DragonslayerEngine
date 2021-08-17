#include "DeferredRenderQueue.h"
#include <assert.h>

using namespace renderer;

bool renderer::DeferredRenderQueue::enqueueRender(RenderCommand renderCommand)
{

	if (!renderCommand.isValid()) {
		return false;
	}

	geometryQueue.push(renderCommand);

	if (renderCommand.castShadows)
		shadowMapQueue.push(renderCommand);

	if (renderCommand.blending) 
		alphaTestQueue.push(renderCommand);
	
	// TODO Custom shader support

	return true;
}

bool renderer::DeferredRenderQueue::enqueuePostProcessing(PostProcessingCommand postProcessingCommand)
{
	if (!postProcessingCommand.isValid()) {
		return false;
	}

	// TODO Post processing

	return true;
}

inline bool renderer::DeferredRenderQueue::isGeometryEmpty()
{
	return geometryQueue.empty();
}

inline RenderCommand renderer::DeferredRenderQueue::dequeueGeometry()
{
	assert(geometryQueue.empty());

	RenderCommand command= geometryQueue.front();
	geometryQueue.pop();
	return command;
}

inline bool renderer::DeferredRenderQueue::isShadowMapEmpty()
{
	return shadowMapQueue.empty();
}

inline RenderCommand renderer::DeferredRenderQueue::dequeueShadowMap()
{
	assert(shadowMapQueue.empty());
	
	RenderCommand command = shadowMapQueue.front();
	shadowMapQueue.pop();
	return command;
}

inline bool renderer::DeferredRenderQueue::isAlphaTestEmpty()
{
	return alphaTestQueue.empty();
}

inline RenderCommand renderer::DeferredRenderQueue::dequeueAlphaTest()
{
	assert(alphaTestQueue.empty());
		
	RenderCommand command = alphaTestQueue.front();
	alphaTestQueue.pop();
	return command;
}

inline bool renderer::DeferredRenderQueue::isCustomShadersEmpty()
{
	return customShadersQueue.empty();
}

inline RenderCommand renderer::DeferredRenderQueue::dequeueCustomShaders()
{
	assert(customShadersQueue.empty());
		
	RenderCommand command = customShadersQueue.front();
	customShadersQueue.pop();
	return command;
}

inline bool renderer::DeferredRenderQueue::isPostProcessingEmpty()
{
	return postProcessingQueue.empty();
}

inline PostProcessingCommand renderer::DeferredRenderQueue::dequeuePostProcessing()
{
	assert(postProcessingQueue.empty());

	PostProcessingCommand command = postProcessingQueue.front();
	postProcessingQueue.pop();
	return command;
}

void renderer::DeferredRenderQueue::clear()
{
	std::queue<RenderCommand, std::deque<RenderCommand>> empty;
	geometryQueue.swap(empty);

	std::queue<RenderCommand, std::deque<RenderCommand>> empty1;
	shadowMapQueue.swap(empty1);

	std::queue<RenderCommand, std::deque<RenderCommand>> empty2;
	alphaTestQueue.swap(empty2);

	std::queue<RenderCommand, std::deque<RenderCommand>> empty3;
	customShadersQueue.swap(empty3);

	std::queue<PostProcessingCommand, std::deque<PostProcessingCommand>> empty4;
	postProcessingQueue.swap(empty4);
}