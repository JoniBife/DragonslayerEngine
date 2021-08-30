#include "../math/Vec3.h"
#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

struct DirectionalLight {

	Vec3 direction = { -1.0f , -1.0f , 0.0f };
	Vec3 color = { 1.0f, 1.0f, 1.0f };
	float intensity = 1.0f;
	bool castShadows = true;

};

#endif