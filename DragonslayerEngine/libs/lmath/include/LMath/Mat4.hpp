#ifndef MATRIX_4_H
#define MATRIX_4_H


#include <iostream>
#include "Vec4.hpp"
#include "Vec3.hpp"
#include "Mat3.hpp"

namespace LMath {

	struct Mat4 {

		float m[4][4];

		Mat4();
		Mat4(const float fill);
		Mat4(const float l1c1, const float l1c2, const float l1c3, const float l1c4,
			const float l2c1, const float l2c2, const float l2c3, const float l2c4,
			const float l3c1, const float l3c2, const float l3c3, const float l3c4,
			const float l4c1, const float l4c2, const float l4c3, const float l4c4);
		Mat4(const Mat4& other);

		static Mat4 IDENTITY;
		static Mat4 ZERO;
		static Mat4 scaling(const float xyz);
		static Mat4 scaling(const float x, const float y, const float z);
		static Mat4 scaling(const Vec3& v);
		static Mat4 translation(const float x, const float y, const float z);
		static Mat4 translation(const Vec3& v);
		static Mat4 rotation(const float angleRad, const Vec3& axis);
		static Mat4 rotationFromDir(const Vec3& dir, const Vec3& up = { 0.0f, 1.0f, 0.0f });

		Mat4& operator=(const Mat4& other);
		Mat4& operator+=(const Mat4& other);
		Mat4& operator-=(const Mat4& other);
		Mat4& operator*=(const Mat4& other);
		Mat4& operator*=(const float s);
		Mat4& operator/=(const float s);
		Mat4& operator+=(const float s);
		Mat4& operator-=(const float s);

		bool operator==(const Mat4& other) const;
		bool operator!=(const Mat4& other) const;

		Mat4 operator+(const Mat4& other) const;
		Mat4 operator-(const Mat4& other) const;
		Mat4 operator*(const Mat4& other)const;
		Mat4 operator*(const float s) const;
		friend Mat4 operator*(const float s, const Mat4& mat4);
		Mat4 operator+(const float s) const;
		friend Mat4 operator+(const float s, const Mat4& mat4);
		Mat4 operator-(const float s) const;
		friend Mat4 operator-(const float s, const Mat4& mat4);
		Mat4 operator/(const float s) const;
		Vec4 operator*(const Vec4& v) const;
		Vec3 operator*(const Vec3& v) const; // This operation is incorrect from linear algebra's perspective but its useful

		float* operator[](const int lines);

		/*
		* Returns false if this is not invertible else true;
		* @param inverse will contain the inverse matrix of this
		*/
		bool inverse(Mat4& inverse) const;
		Mat4 transpose() const;
		void toOpenGLFormat(float array[16]) const;

		Mat3 toMat3() const; //removes last line and column
		bool isOrthogonal() const;

		/* Decomposes a transformation matrix in each of its components */
		void decompose(Vec3& scale, Vec3& rotation, Vec3& position) const;

		friend std::ostream& operator<<(std::ostream& os, const Mat4& mat4);
		friend std::istream& operator>>(std::istream& is, Mat4& mat4);

	};
}
#endif

