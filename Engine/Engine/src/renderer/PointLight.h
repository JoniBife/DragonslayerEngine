#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "../math/Vec3.h"

namespace renderer {

	struct PointLight {
		Vec3 position;
		Vec3 color = { 1.0f, 1.0f, 1.0f };
		float radiance = 1.0f;
		float radius = 5.0f;
	};

}


#endif
