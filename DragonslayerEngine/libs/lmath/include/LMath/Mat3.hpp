#ifndef MATRIX_3_H
#define MATRIX_3_H

#include <iostream>
#include "Vec3.hpp"

namespace LMath {

	struct Mat3 {

		float m[3][3];

		Mat3();
		Mat3(const float fill);
		Mat3(const float l1c1, const float l1c2, const float l1c3,
			const float l2c1, const float l2c2, const float l2c3,
			const float l3c1, const float l3c2, const float l3c3);
		Mat3(const Mat3& other);

		static Mat3 ZERO;
		static Mat3 IDENTITY;
		static Mat3 dual(const Vec3& v);
		static Mat3 randomMatrix(float min, float max);

		Mat3& operator=(const Mat3& other);
		Mat3& operator+=(const Mat3& other);
		Mat3& operator-=(const Mat3& other);
		Mat3& operator*=(const Mat3& other);
		Mat3& operator*=(const float s);
		Mat3& operator/=(const float s);
		Mat3& operator+=(const float s);
		Mat3& operator-=(const float s);

		bool operator==(const Mat3& other) const;
		bool operator!=(const Mat3& other) const;

		Mat3 operator+(const Mat3& other) const;
		Mat3 operator-(const Mat3& other) const;
		Mat3 operator*(const Mat3& other) const;
		Mat3 operator*(const float s) const;
		friend Mat3 operator*(const float s, const Mat3& mat3);
		Mat3 operator+(const float s) const;
		friend Mat3 operator+(const float s, const Mat3& mat3);
		Mat3 operator-(const float s) const;
		friend Mat3 operator-(const float s, const Mat3& mat3);
		Mat3 operator/(const float s) const;
		Vec3 operator*(const Vec3& v) const;

		float* operator[](const int lines);

		float determinant() const;
		Mat3 transpose() const;
		/*
		* Returns false if this is not invertible else true;
		* @param inverse will contain the inverse matrix of this
		*/
		bool inverse(Mat3& inverse) const;
		void toOpenGLFormat(float array[9]) const;
		bool isOrthogonal() const;

		friend std::ostream& operator<<(std::ostream& os, const Mat3& mat3);
		friend std::istream& operator>>(std::istream& is, Mat3& mat3);

	};

}
#endif