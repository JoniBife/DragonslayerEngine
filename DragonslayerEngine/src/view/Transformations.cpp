#include "Transformations.h"
#include <cmath>
#include <cassert>

Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {

	Vec3 v = (center - eye).normalize();
	Vec3 s = cross(v, up).normalize();
	Vec3 u = cross(s, v);

	return {
		s.x, s.y, s.z, -dot(s, eye),
		u.x, u.y, u.z, -dot(u, eye),
		-v.x, -v.y, -v.z, dot(v, eye),
		0,  0,  0,  1
	};
}

Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {

	// Divisions by 0 are not possible
	assert(left != right && bottom != top && far != near );

	return { 2.0f / (right - left) , 0.0f, 0.0f, (left + right) / (left - right),
			0.0f, 2.0f/(top-bottom), 0.0f, (bottom + top)/(bottom - top), 
			0.0f, 0.0f, 2.0f/(near-far), (near+far)/(near-far), 
			0.0f, 0.0f, 0.0f, 1.0f
	};
}

Mat4 ortho(float left, float right, float bottom, float top) {

	// Divisions by 0 are not possible
	assert(left != right && bottom != top);

	return { 2.0f / (right - left) , 0.0f, 0.0f, (left + right) / (left - right),
			0.0f, 2.0f / (top - bottom), 0.0f, (bottom + top) / (bottom - top),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};
}

Mat4 perspective(float fovyRad, float aspectRatio, float near, float far) {


	// Divisions by 0 are not possible
	assert(aspectRatio != 0);
	assert(aspectRatio < 180.0);
	assert(near != far);

	float theta = fovyRad / 2.0f;
	float d = 1.0f / tanf(theta);

	float diffNearFar = near - far;

	Mat4 ret =  { d / aspectRatio, 0.0f , 0.0f, 0.0f,
			0.0f, d, 0.0f, 0.0f,
			0.0f, 0.0f, (near + far) / diffNearFar, (2.0f * near * far) / diffNearFar,
			0.0f, 0.0f, -1.0f, 0.0f };
	
	return ret;
}

Mat4 orthoCascade(float nearViewSpace, float farViewSpace, float fovRad, float aspectRatio, const Mat4& inverseCameraView, const Mat4& lightView)
{
	assert(farViewSpace > nearViewSpace);

	// First we find the coordinates of the min and max points that define the bounding box
	float halfFov = fovRad * 0.5f;
	float w = tanf(halfFov) * farViewSpace / aspectRatio;
	float h = tanf(halfFov) * farViewSpace;

	// Defining the bounding box that captures this area of the view frustrum
	// which contains the cascade
	Vec4 corners[8];
	// Top vertices
	corners[0] = { -w, h, -nearViewSpace , 1.0f };
	corners[1] = { -w, h, -farViewSpace, 1.0f };
	corners[2] = { w, h, -nearViewSpace, 1.0f };
	corners[3] = { w, h, -farViewSpace, 1.0f };
	// Bottom vertices
	corners[4] = { -w, -h, -nearViewSpace , 1.0f };
	corners[5] = { -w, -h, -farViewSpace, 1.0f };
	corners[6] = { w, -h, -nearViewSpace, 1.0f };
	corners[7] = { w, -h, -farViewSpace, 1.0f };

	float far = std::numeric_limits<float>::max();
	float near = std::numeric_limits<float>::min();
	float left = std::numeric_limits<float>::max();
	float right = std::numeric_limits<float>::min();
	float bottom = std::numeric_limits<float>::max();
	float top = std::numeric_limits<float>::min();

	Vec4 cornerLightSpace;

	//Iterating over the corners to find all the planes that encapsulate the bounding box
	for (Vec4& corner : corners) {

		cornerLightSpace = lightView * inverseCameraView * corner;

		right = fmaxf(right, cornerLightSpace.x);
		left = fminf(left, cornerLightSpace.x);
		top = fmaxf(top, cornerLightSpace.y);
		bottom = fminf(bottom, cornerLightSpace.y);
		far = fminf(far, cornerLightSpace.z); // Inverted because the forward direction is -Z
		near = fmaxf(near, cornerLightSpace.z); // Inverted because the forward direction is -Z
	}

	// Near and far sign is altered again because ortho expects them positive and inverts them internally
	return ortho(left, right, bottom, top, -near, -far);
}
