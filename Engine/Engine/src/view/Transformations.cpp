
#include "Transformations.h"
#include <math.h>
#include <assert.h>
#include <initializer_list>

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

	//glm::mat4 persp = glm::perspective(fovyRad, aspectRatio, near, far);

	// Divisions by 0 are not possible
	assert(aspectRatio != 0);
	assert(near != far);

	float theta = fovyRad / 2;
	float d = 1 / tanf(theta);

	float diffNearFar = near - far;

	Mat4 ret =  { d / aspectRatio, 0.0f , 0.0f, 0.0f,
			0.0f, d, 0.0f, 0.0f,
			0.0f, 0.0f, (near + far) / diffNearFar, (2 * near * far) / diffNearFar,
			0.0f, 0.0f, -1.0f, 0.0f };
	
	return ret;
}

Mat4 orthoCascade(const Camera& camera, float nearViewSpace, float farViewSpace, float fovRad, float inverseAspectRatio, Mat4 inverseCameraView, Mat4 lightView, Vec4& outTopRight, Vec4& outBottomLeft)
{
	assert(farViewSpace > nearViewSpace);

	// First we find the coordinates of the min and max points that define the bounding box
	// Trignometry explained here: https://ogldev.org/www/tutorial49/tutorial49.html
	float halfFov = fovRad * 0.5f;
	float x1 = tanf(halfFov) * nearViewSpace * camera.getAspectRatio();
	float x2 = tanf(halfFov) * farViewSpace * camera.getAspectRatio();

	// Height is typically smaller than the width so we have to ajust the FOV
	float y1 = tanf(halfFov) * nearViewSpace;
	float y2 = tanf(halfFov) * farViewSpace;

	// Top right and top left corners of bounding box in view space
	Vec4 topRight = { x2, y2, farViewSpace , 1.0f};
	Vec4 bottomLeft = { -x1, -y1, nearViewSpace , 1.0f};
	
	Vec3 farCenter = camera.getPosition() + camera.getFront().normalize() * farViewSpace;
	farCenter = farCenter + camera.getUp() * y1 + camera.getRight() * x1;
	Vec3 nearCenter = camera.getPosition() + camera.getFront().normalize() * nearViewSpace;
	nearCenter = nearCenter - camera.getUp() * y1 - camera.getRight() * y1;

	Vec4 cameraPosition(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, 1.0f);
	outTopRight = Vec4(farCenter.x, farCenter.y, farCenter.z, 1.0f);
	outBottomLeft = Vec4(nearCenter.x, nearCenter.y, nearCenter.z, 1.0f);

	// The corners are now moved back to world space and then to light space
	topRight = lightView * inverseCameraView * topRight;
	bottomLeft = lightView * inverseCameraView * bottomLeft;

	float far = topRight.z;
	float near = bottomLeft.z;
	float left = bottomLeft.x;
	float right = topRight.x;
	float bottom = bottomLeft.y;
	float top = topRight.y;

	if (topRight.z > bottomLeft.z) {
		far = bottomLeft.z;
		near = topRight.z;
	}

	if (topRight.x < bottomLeft.x) {
		right = bottomLeft.x;
		left = topRight.x;
	}

	if (topRight.y < bottomLeft.y) {
		bottom = topRight.y;
		top = bottomLeft.y;
	}

	return ortho(left, right, bottom, top, near, far);
}
