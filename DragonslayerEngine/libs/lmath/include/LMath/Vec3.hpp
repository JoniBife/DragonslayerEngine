#ifndef VEC3_H
#define VEC3_H

#include <ostream>
#include "Vec4.hpp"

namespace LMath {

	struct Vec3 {

		float x, y, z;

		Vec3();
		Vec3(float xyz);
		Vec3(float x, float y, float z);

		static Vec3 ZERO;
		static Vec3 Z;
		static Vec3 Y;
		static Vec3 X;

		static Vec3 random();

		Vec3 operator+(const Vec3& other) const;
		Vec3 operator-(const Vec3& other) const;
		Vec3& operator=(const Vec3& other);
		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);
		Vec3& operator*=(const Vec3& other);
		Vec3& operator/=(const Vec3& other);

		Vec3 operator*(float scalar) const;
		friend Vec3 operator*(float scalar, const Vec3& vec3);
		Vec3 operator/(float scalar) const;
		Vec3 operator+(float scalar) const;
		friend Vec3 operator+(float scalar, const Vec3& vec3);
		Vec3 operator-(float scalar) const;
		friend Vec3 operator-(float scalar, const Vec3& vec3);

		Vec3& operator+=(float s);
		Vec3& operator-=(float s);
		Vec3& operator*=(float s);
		Vec3& operator/=(float s);

		bool operator==(const Vec3& other) const;
		bool operator!=(const Vec3& other) const;

		static float angle(const Vec3& a, const Vec3& b);

		float magnitude() const;
		float sqrMagnitude() const;

		void toOpenGLFormat(float array[3]) const;

		static Vec3 lerp(Vec3 initial, Vec3 final, float f);

		Vec3 normalize() const;
		Vec4 toVec4() const;
	};

	float dot(const Vec3& a, const Vec3& b);
	Vec3 cross(const Vec3& a, const Vec3& b);
	Vec3 rodrigues(Vec3 v, float theta, Vec3 k);
	std::ostream& operator<<(std::ostream& os, const Vec3& vec3);

}

#endif