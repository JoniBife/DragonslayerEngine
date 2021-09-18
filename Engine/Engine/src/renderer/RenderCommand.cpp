#include "RenderCommand.h"

bool WarriorRenderer::RenderCommand::isValid() const
{
	return mesh != nullptr && material != nullptr;
}
