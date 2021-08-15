#include "DeferredRenderQueue.h"

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
	if (geometryQueue.empty()) 
		throw "Could not dequeue! Geometry queue is empty, check with isGeometryEmpty() before dequeueing.";
	
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
	if (shadowMapQueue.empty())
		throw "Could not dequeue! Shadow Mapping queue is empty, check with isShadowMapEmpty() before dequeueing.";;
	
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
	if (alphaTestQueue.empty())
		throw "Could not dequeue! Alpha testing queue is empty, check with isAlphaTestEmpty() before dequeueing.";

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
	if (customShadersQueue.empty())
		throw "Could not dequeue! Custom shaders queue is empty, check with isCustomShadersEmpty() before dequeueing.";

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
	if (postProcessingQueue.empty())
		throw "Could not dequeue! Post processing queue is empty, check with isPostProcessingEmpty() before dequeueing.";

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
