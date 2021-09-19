#include "RenderQueue.h"
#include <assert.h>

using namespace WarriorRenderer;

bool WarriorRenderer::RenderQueue::enqueueRender(RenderCommand* renderCommand)
{
	if (!renderCommand->isValid()) {
		return false;
	}

	geometryQueue.push(renderCommand);

	if (renderCommand->castShadows)
		shadowMapQueue.push(renderCommand);

	// TODO Uncomment when implementing alpha testing
	/*if (renderCommand.blending)
		alphaTestQueue.push(renderCommand);*/
	
	// TODO Custom shader support

	return true;
}

bool WarriorRenderer::RenderQueue::enqueuePostProcessing(PostProcessingCommand* const postProcessingCommand)
{
	if (!postProcessingCommand->isValid()) {
		return false;
	}

	postProcessingQueue.push(postProcessingCommand);

	// TODO Post processing

	return true;
}

bool WarriorRenderer::RenderQueue::isGeometryEmpty()
{
	return geometryQueue.empty();
}

RenderCommand& WarriorRenderer::RenderQueue::dequeueGeometry()
{
	assert(!geometryQueue.empty());

	RenderCommand* command= geometryQueue.front();
	geometryQueue.pop();
	return *command;
}

bool WarriorRenderer::RenderQueue::isShadowMapEmpty()
{
	return shadowMapQueue.empty();
}

RenderCommand& WarriorRenderer::RenderQueue::dequeueShadowMap()
{
	assert(!shadowMapQueue.empty());
	
	RenderCommand* command = shadowMapQueue.front();
	shadowMapQueue.pop();
	return *command;
}

std::queue<RenderCommand*>& WarriorRenderer::RenderQueue::getShadowMapQueue()
{
	return shadowMapQueue;
}

bool WarriorRenderer::RenderQueue::isAlphaTestEmpty()
{
	return alphaTestQueue.empty();
}

RenderCommand& WarriorRenderer::RenderQueue::dequeueAlphaTest()
{
	assert(!alphaTestQueue.empty());
		
	RenderCommand* command = alphaTestQueue.front();
	alphaTestQueue.pop();
	return *command;
}

bool WarriorRenderer::RenderQueue::isCustomShadersEmpty()
{
	return customShadersQueue.empty();
}

RenderCommand& WarriorRenderer::RenderQueue::dequeueCustomShaders()
{
	assert(!customShadersQueue.empty());
		
	RenderCommand* command = customShadersQueue.front();
	customShadersQueue.pop();
	return *command;
}

bool WarriorRenderer::RenderQueue::isPostProcessingEmpty()
{
	return postProcessingQueue.empty();
}

PostProcessingCommand& WarriorRenderer::RenderQueue::dequeuePostProcessing()
{
	assert(!postProcessingQueue.empty());

	PostProcessingCommand* command = postProcessingQueue.front();
	postProcessingQueue.pop();
	return *command;
}

void WarriorRenderer::RenderQueue::clear()
{
	std::queue<RenderCommand*, std::deque<RenderCommand*>> empty;
	geometryQueue.swap(empty);

	std::queue<RenderCommand*, std::deque<RenderCommand*>> empty1;
	shadowMapQueue.swap(empty1);

	std::queue<RenderCommand*, std::deque<RenderCommand*>> empty2;
	alphaTestQueue.swap(empty2);

	std::queue<RenderCommand*, std::deque<RenderCommand*>> empty3;
	customShadersQueue.swap(empty3);

	std::queue<PostProcessingCommand*, std::deque<PostProcessingCommand*>> empty4;
	postProcessingQueue.swap(empty4);
}
