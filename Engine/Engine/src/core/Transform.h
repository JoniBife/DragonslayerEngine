#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <list>
#include "../math/Vec3.h"
#include "../math/Mat4.h"

namespace core {

	/*
	* TODO More accurate names
	* Maybe use unity example
	*/
	class Transform {

	private:
		Transform();
		Mat4 model = Mat4::IDENTITY;
		
	public:
		Vec3 position = Vec3::ZERO; // Position in world space
		Vec3 rotation = Vec3::ZERO; // Local rotation
		Vec3 scale = {1.0f, 1.0f, 1.0f}; // Local Scale

		Mat4 getModelMatrix();

		void onGUI();

		/* Updates the position rotation and scale to be relative from the parent */
		void onFrameUpdate(const Mat4& parentModel);

		void makeChildOfTransform(Transform* transform);
		void removeChildOfTransform(Transform* transform);

		friend class GameObject;

	};

};

#endif
