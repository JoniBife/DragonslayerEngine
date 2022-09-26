#include "DragonslayerEngine/RenderCommand.h"

bool DragonslayerEngine::RenderCommand::isValid() const
{
	return mesh && material;
}
