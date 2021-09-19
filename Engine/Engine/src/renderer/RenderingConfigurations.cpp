#include "RenderingConfigurations.h"
#include <cassert>

bool RenderingConfigurations::areValid()
{
	bool areValid = true;
	areValid &= numberOfCascades > 0u;

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
