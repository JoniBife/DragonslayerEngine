#include "RenderCommand.h"

using namespace renderer;

bool renderer::RenderCommand::isValid() const
{
	return mesh != nullptr && material != nullptr;
}
