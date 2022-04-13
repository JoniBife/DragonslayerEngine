#include "DragonslayerEngine/RenderQueue.h"
#include <assert.h>

using namespace DragonslayerEngine;

bool DragonslayerEngine::RenderQueue::enqueueRender(RenderCommand* renderCommand)
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

bool DragonslayerEngine::RenderQueue::enqueuePostProcessing(PostProcessingCommand* const postProcessingCommand)
{
	if (!postProcessingCommand->isValid()) {
		return false;
	}

	postProcessingQueue.push(postProcessingCommand);

	return true;
}

bool DragonslayerEngine::RenderQueue::isGeometryEmpty()
{
	return geometryQueue.empty();
}

RenderCommand& DragonslayerEngine::RenderQueue::dequeueGeometry()
{
	assert(!geometryQueue.empty());

	RenderCommand* command= geometryQueue.front();
	geometryQueue.pop();
	return *command;
}

bool DragonslayerEngine::RenderQueue::isShadowMapEmpty()
{
	return shadowMapQueue.empty();
}

RenderCommand& DragonslayerEngine::RenderQueue::dequeueShadowMap()
{
	assert(!shadowMapQueue.empty());
	
	RenderCommand* command = shadowMapQueue.front();
	shadowMapQueue.pop();
	return *command;
}

std::queue<RenderCommand*>& DragonslayerEngine::RenderQueue::getShadowMapQueue()
{
	return shadowMapQueue;
}

void DragonslayerEngine::RenderQueue::clearShadowMapQueue()
{
	std::queue<RenderCommand*, std::deque<RenderCommand*>> empty;
	shadowMapQueue.swap(empty);
}

bool DragonslayerEngine::RenderQueue::isAlphaTestEmpty()
{
	return alphaTestQueue.empty();
}

RenderCommand& DragonslayerEngine::RenderQueue::dequeueAlphaTest()
{
	assert(!alphaTestQueue.empty());
		
	RenderCommand* command = alphaTestQueue.front();
	alphaTestQueue.pop();
	return *command;
}

bool DragonslayerEngine::RenderQueue::isCustomShadersEmpty()
{
	return customShadersQueue.empty();
}

RenderCommand& DragonslayerEngine::RenderQueue::dequeueCustomShaders()
{
	assert(!customShadersQueue.empty());
		
	RenderCommand* command = customShadersQueue.front();
	customShadersQueue.pop();
	return *command;
}

bool DragonslayerEngine::RenderQueue::isPostProcessingEmpty()
{
	return postProcessingQueue.empty();
}

PostProcessingCommand& DragonslayerEngine::RenderQueue::dequeuePostProcessing()
{
	assert(!postProcessingQueue.empty());

	PostProcessingCommand* command = postProcessingQueue.front();
	postProcessingQueue.pop();
	return *command;
}

void DragonslayerEngine::RenderQueue::clear()
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
