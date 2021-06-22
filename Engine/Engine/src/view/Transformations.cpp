#include "Transformations.h"
#include <math.h>
#include <assert.h>

Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {

	Vec3 v = (center - eye).normalize();
	Vec3 s = cross(v, up).normalize();
	Vec3 u = cross(s, v);

	return {
		s.x, s.y, s.z, dot(s * -1, eye),
		u.x, u.y, u.z, dot(u * -1, eye),
		-v.x, -v.y, -v.z, dot(v, eye),
		0,  0,  0,  1
	};
}

Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {

	// Divisions by 0 are not possible
	assert(left != right && bottom != top && top != near );

	return { 2 / (right - left) , 0.0f, 0.0f, (left + right) / (left - right),
			0.0f, 2/(top-bottom), 0.0f, (bottom + top)/(bottom - top), 
			0.0f, 0.0f, 2/(near-far), (near+far)/(near-far), 
			0.0f, 0.0f, 0.0f, 1.0f
	};
}

Mat4 ortho(float left, float right, float bottom, float top) {

	// Divisions by 0 are not possible
	assert(left != right && bottom != top);

	return { 2 / (right - left) , 0.0f, 0.0f, (left + right) / (left - right),
			0.0f, 2 / (top - bottom), 0.0f, (bottom + top) / (bottom - top),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};
}

Mat4 perspective(float fovyRad, float aspectRatio, float near, float far) {

	// Divisions by 0 are not possible
	assert(aspectRatio > 0);
	assert(near != far);

	float theta = fovyRad / 2;
	float d = 1 / tanf(theta);

	float diffNearFar = near - far;

	return { d / aspectRatio, 0.0f , 0.0f, 0.0f,
			0.0f, d, 0.0f, 0.0f,
			0.0f, 0.0f, (near + far) / diffNearFar, (2 * near * far) / diffNearFar,
			0.0f, 0.0f, -1.0f, 0.0f };
}