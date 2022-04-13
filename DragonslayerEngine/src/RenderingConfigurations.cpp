#include "DragonslayerEngine/RenderingConfigurations.h"
#include <cassert>

RenderingConfigurations::RenderingConfigurations() {




}


// TODO Better validations
bool RenderingConfigurations::areValid() const
{
	bool areValid = true;
	areValid &= cascadesPlanes.size() > 0u && cascadesPlanes.size() < MAX_CASCADES;

	float previousPlane = 0u;
	for (float cascadePlane : cascadesPlanes) {
		areValid &= cascadePlane > 0u;
		areValid &= cascadePlane > previousPlane;
		previousPlane = cascadePlane;
	}

	areValid &= shadowsResolution > 0u;
	areValid &= renderWidth > 0u && renderHeight > 0u;
	return areValid;
}
