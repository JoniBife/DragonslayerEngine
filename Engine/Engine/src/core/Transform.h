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
		Vec3 position = Vec3::ZERO; 
		Vec3 rotation = Vec3::ZERO; 
		Vec3 scale = {1.0f, 1.0f, 1.0f}; 

		Vec3 localPosition = Vec3::ZERO; // Position relative to parent
		Vec3 localRotation = Vec3::ZERO; // Rotation relative to parent
		Vec3 localScale = { 1.0f, 1.0f, 1.0f }; // Scale relative to parent

		Mat4 getModelMatrix();

		void onGUI();
		void onGUI(const Transform& parent);

		/* Updates the position rotation and scale to be relative from the parent */
		void onFrameUpdate(const Mat4& parentModel);

		void makeChildOfTransform(Transform* transform);
		void removeChildOfTransform(Transform* transform);

		/* Returns the Y axis of the transform in world space*/
		Vec3 getUp() const;
		/* Returns the Z axis of the transform in world space*/
		Vec3 getForward() const;
		/* Returns the X axis of the transform in world space*/
		Vec3 getRight() const;

		friend class GameObject;

	};

}

#endif
