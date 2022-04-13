#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <LMath/Vec3.hpp>

using namespace LMath;

namespace DragonslayerEngine {

	struct DirectionalLight {

		Vec3 direction = { -1.0f , -1.0f , 0.0f }; // Is always normalized before sending to the fragment shader
		Vec3 color = { 1.0f, 1.0f, 1.0f };
		float intensity = 1.0f;
		bool castShadows = true;

	};

}

#endif