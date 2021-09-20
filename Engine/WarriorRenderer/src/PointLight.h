#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "math/Vec3.h"

namespace WarriorRenderer {

	struct PointLight {
		Vec3 position;
		float radiance = 1.0f;
		Vec3 color = { 1.0f, 1.0f, 1.0f };
		float radius = 5.0f;
	};

}


#endif
