#include "RenderQueue.h"

using namespace renderer;

bool renderer::RenderCommand::isValid() const
{
	return mesh != nullptr && material != nullptr;
}

bool renderer::PostProcessingCommand::isValid() const
{
	return false;
}
