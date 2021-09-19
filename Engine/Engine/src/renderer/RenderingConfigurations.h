#ifndef RENDERING_CONFIGURATIONS_H
#define RENDERING_CONFIGURATIONS_H

#include <vector>

struct RenderingConfigurations {
	
	unsigned int numberOfCascades = 3u;
	std::vector<float> cascadesPlanes = { 7.5f, 25.0f, 42.5f };

	unsigned int shadowsResolution = 1024u;

	unsigned int renderWidth = 1366u;
	unsigned int renderHeight = 768u;
	
	bool areValid();
};

#endif
