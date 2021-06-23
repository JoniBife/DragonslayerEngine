#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math/Vec3.h"

/*
* TODO More accurate names
* Maybe use unity example
*/
struct Transform {
	Vec3 position; // Position in world space
	Vec3 rotation; // Local rotation
	Vec3 scale; // Local Scala
};

#endif
