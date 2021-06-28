#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <list>
#include "../math/Vec3.h"

namespace core {

	/*
	* TODO More accurate names
	* Maybe use unity example
	*/
	class Transform {

	private:
		Transform();
		
	public:
		Vec3 position = Vec3::ZERO; // Position in world space
		Vec3 rotation = Vec3::ZERO; // Local rotation
		Vec3 scale = {1.0f, 1.0f, 1.0f}; // Local Scale

		friend class GameObject;

	};

};

#endif
