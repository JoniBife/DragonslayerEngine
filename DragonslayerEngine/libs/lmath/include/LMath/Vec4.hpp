#ifndef VEC4_H
#define VEC4_H

#include <iostream>

namespace LMath {

	struct Vec4 {

		float x, y, z, w;

		Vec4();
		Vec4(float xyz);
		Vec4(float x, float y, float z, float w);

		static Vec4 ZERO;
		static Vec4 Z;
		static Vec4 Y;
		static Vec4 X;

		Vec4 operator+(const Vec4& other) const;
		Vec4 operator-(const Vec4& other) const;
		Vec4& operator=(const Vec4& other);
		Vec4& operator+=(const Vec4& other);
		Vec4& operator-=(const Vec4& other);

		Vec4 operator*(float scalar) const;
		friend Vec4 operator*(float scalar, const Vec4& vec4);
		Vec4 operator/(float scalar) const;
		Vec4 operator+(float scalar) const;
		friend Vec4 operator+(float scalar, const Vec4& vec4);
		Vec4 operator-(float scalar) const;
		friend Vec4 operator-(float scalar, const Vec4& vec4);

		Vec4& operator+=(float s);
		Vec4& operator-=(float s);
		Vec4& operator*=(float s);
		Vec4& operator/=(float s);

		bool operator==(const Vec4& other) const;
		bool operator!=(const Vec4& other) const;

		float magnitude() const;
		float sqrMagnitude() const;
		Vec4 normalize() const;

		void toOpenGLFormat(float array[4]) const;
	};

	float dot(const Vec4& a, const Vec4& b);
	std::ostream& operator<<(std::ostream& os, const Vec4& vec4);

}

#endif