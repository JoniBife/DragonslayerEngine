
#include "Transformations.h"
#include <math.h>
#include <assert.h>
#include <initializer_list>
#include <limits>

Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {

	Vec3 v = (center - eye).normalize();
	Vec3 s = cross(v, up).normalize();
	Vec3 u = cross(s, v);

	return {
		s.x, s.y, s.z, dot(s * -1.0f, eye),
		u.x, u.y, u.z, dot(u * -1.0f, eye),
		-v.x, -v.y, -v.z, dot(v, eye),
		0,  0,  0,  1
	};
}

Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {

	// Divisions by 0 are not possible
	assert(left != right && bottom != top && top != near );

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

Mat4 orthoCascade(float nearViewSpace, float farViewSpace, float fovRad, float inverseAspectRatio, Mat4 inverseCameraView, Mat4 lightView)
{
	assert(farViewSpace > nearViewSpace);

	// First we find the coordinates of the min and max points that define the bounding box
	// Trignometry explained here: https://ogldev.org/www/tutorial49/tutorial49.html
	float halfFov = fovRad * 0.5f;
	float x1 = tanf(halfFov) * nearViewSpace * inverseAspectRatio; // TODO Check if this multiplication is correct
	float x2 = tanf(halfFov) * farViewSpace * inverseAspectRatio;

	// Height is typically smaller than the width so we have to ajust the FOV
	float y1 = tanf(halfFov) * nearViewSpace;
	float y2 = tanf(halfFov) * farViewSpace;

	// Defining the bounding box that captures this area of the view frustrum
	// which contains the cascade
	Vec4 corners[8];
	// Top vertices
	corners[0] = { -x2, y2, -nearViewSpace , 1.0f };
	corners[1] = { -x2, y2, -farViewSpace, 1.0f };
	corners[2] = { x2, y2, -nearViewSpace, 1.0f };
	corners[3] = { x2, y2, -farViewSpace, 1.0f };
	// Bottom vertices
	corners[4] = { -x2, -y2, -nearViewSpace , 1.0f };
	corners[5] = { -x2, -y2, -farViewSpace, 1.0f };
	corners[6] = { x2, -y2, -nearViewSpace, 1.0f };
	corners[7] = { x2, -y2, -farViewSpace, 1.0f };

	float far = corners[0].z;
	float near = corners[0].z;
	float left = corners[0].x;
	float right = corners[0].x;
	float bottom = corners[0].y;
	float top = corners[0].y;

	Vec4 cornerLightSpace;

	//Iterating over the corners to find all of the planes that incapsulate the bounding box
	for (Vec4 corner : corners) {

		cornerLightSpace = lightView * inverseCameraView * corner;

		right = fmaxf(right, cornerLightSpace.x);
		left = fminf(left, cornerLightSpace.x);
		top = fmaxf(top, cornerLightSpace.y);
		bottom = fminf(bottom, cornerLightSpace.y);
		far = fminf(far, cornerLightSpace.z); // Inverted because the forward direction is -Z
		near = fmaxf(near, cornerLightSpace.z); // Inverted because the forward direction is -Z
	}

	// Near and far sign is altered again because ortho expects them positive and inverts them internally
	return ortho(roundf(left), roundf(right), roundf(bottom), roundf(top), near * -1, far * -1);
}
