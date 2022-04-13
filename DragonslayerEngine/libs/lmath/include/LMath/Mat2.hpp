#ifndef MAT_2_H
#define MAT_2_H

#include "Vec2.hpp"

namespace LMath {

	struct Mat2 {
		float m[2][2];

		Mat2();
		Mat2(const float fill);
		Mat2(const float l1c1, const float l1c2,
			const float l2c1, const float l2c2);
		Mat2(const Mat2& other);

		static Mat2 IDENTITY;

		Mat2& operator=(const Mat2& other);
		Mat2& operator+=(const Mat2& other);
		Mat2& operator-=(const Mat2& other);
		Mat2& operator*=(const Mat2& other);
		Mat2& operator*=(const float s);
		Mat2& operator/=(const float s);
		Mat2& operator+=(const float s);
		Mat2& operator-=(const float s);

		bool operator==(const Mat2& other) const;
		bool operator!=(const Mat2& other) const;

		Mat2 operator+(const Mat2& other) const;
		Mat2 operator-(const Mat2& other) const;
		Mat2 operator*(const Mat2& other) const;
		Mat2 operator*(const float s) const;
		friend Mat2 operator*(const float s, const Mat2& mat2);
		Mat2 operator+(const float s) const;
		friend Mat2 operator+(const float s, const Mat2& mat2);
		Mat2 operator-(const float s) const;
		friend Mat2 operator-(const float s, const Mat2& mat2);
		Mat2 operator/(const float s) const;
		Vec2 operator*(const Vec2& v) const;

		float* operator[](const int lines);

		Mat2 transpose() const;
		/*
		* Returns false if this is not invertible
		* else true;
		* @param inverse will contain the inverse matrix of this
		*/
		bool inverse(Mat2& inverse) const;
		float determinant() const;
		void toOpenGLFormat(float array[4]) const;
		bool isOrthogonal() const;

		/*
		 * Print result example:
		* [ 1 , 0 ]
		* [ 0 , 1 ]
		*/
		friend std::ostream& operator<<(std::ostream& os, const Mat2& mat2);
		friend std::istream& operator>>(std::istream& is, Mat2& mat2);
	};
}

#endif
