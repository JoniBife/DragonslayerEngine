#include "RenderPipeline.h"

using namespace renderer;

renderer::RenderPipeline::RenderPipeline(RenderQueue* renderQueue) : renderQueue(renderQueue)
{
}

renderer::RenderPipeline::~RenderPipeline()
{
	delete renderQueue;
}

RenderQueue& renderer::RenderPipeline::getRenderQueue() const
{
	return *renderQueue;
}

bool renderer::RenderPipeline::enqueueRender(const RenderCommand& renderCommand)
{
	return renderQueue->enqueueRender(renderCommand);
}

bool renderer::RenderPipeline::enqueuePostProcessing(const PostProcessingCommand& postProcessingCommand)
{
	return renderQueue->enqueuePostProcessing(postProcessingCommand);
}
