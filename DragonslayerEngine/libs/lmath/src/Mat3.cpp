#include <math.h>
#include "LMath\Mat3.hpp"
#include "LMath\MathAux.hpp"

using namespace LMath;

LMath::Mat3::Mat3() : Mat3(0) {}

LMath::Mat3::Mat3(const float fill) : Mat3(fill, fill, fill, fill, fill, fill, fill, fill, fill) {}

LMath::Mat3::Mat3(const float l1c1, const float l1c2, const float l1c3,
	const float l2c1, const float l2c2, const float l2c3,
	const float l3c1, const float l3c2, const float l3c3) {

	m[0][0] = l1c1;
	m[0][1] = l1c2;
	m[0][2] = l1c3;
	m[1][0] = l2c1;
	m[1][1] = l2c2;
	m[1][2] = l2c3;
	m[2][0] = l3c1;
	m[2][1] = l3c2;
	m[2][2] = l3c3;
}

LMath::Mat3::Mat3(const Mat3& other) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] = other.m[l][c];
		}
	}
}

float LMath::Mat3::determinant() const {
	return	m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
			m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
			m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

Mat3 LMath::Mat3::ZERO = { 0, 0, 0,
					0, 0, 0,
					0, 0, 0 };

Mat3 LMath::Mat3::IDENTITY = { 1, 0, 0,
						0, 1, 0,
						0, 0, 1 };

Mat3 LMath::Mat3::dual(const Vec3& v) {
	return { 0 , -v.z,  v.y,
			v.z ,  0  , -v.x,
			-v.y,  v.x,   0
	};
}

Mat3 LMath::Mat3::randomMatrix(float min, float max) {

	Mat3 randomM;

	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			randomM.m[l][c] = randomFloat(min, max);
		}
	}
	return randomM;
 
}

Mat3& LMath::Mat3::operator=(const Mat3& other)
{
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] = other.m[l][c];
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator+=(const Mat3& other)
{
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] += other.m[l][c];
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator-=(const Mat3& other)
{
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] -= other.m[l][c];
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator*=(const Mat3& other)
{
	Mat3 prod;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			prod.m[l][c] =  m[l][0] * other.m[0][c] +
							m[l][1] * other.m[1][c] +
							m[l][2] * other.m[2][c];
		}
	}

	*this = prod;

	return *this;
}

Mat3& LMath::Mat3::operator*=(const float s) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] *= s;
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator/=(const float s) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] /= s;
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator+=(const float s) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] += s;
		}
	}
	return *this;
}

Mat3& LMath::Mat3::operator-=(const float s) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			m[l][c] -= s;
		}
	}
	return *this;
}

bool LMath::Mat3::operator==(const Mat3& other) const {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			if (!cmpf(m[l][c], other.m[l][c]))
				return false;
		}
	}
	return true;
}

bool LMath::Mat3::operator!=(const Mat3& other) const {
	return !(*this == other);
}

Mat3 LMath::Mat3::operator+(const Mat3& other) const {
	Mat3 sum;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			sum.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return sum;
}
Mat3 LMath::Mat3::operator-(const Mat3& other) const {
	Mat3 diff;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			diff.m[l][c] = m[l][c] - other.m[l][c];
		}
	}
	return diff;
}
Mat3 LMath::Mat3::operator*(const Mat3& other) const {
	Mat3 prod;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			prod.m[l][c] =  m[l][0] * other.m[0][c] +
							m[l][1] * other.m[1][c] +
							m[l][2] * other.m[2][c];
		}
	}
	return prod;
}
Mat3 LMath::Mat3::operator*(const float s) const {
	Mat3 prod;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			prod.m[l][c] = m[l][c] * s;
		}
	}
	return prod;
}


Mat3 LMath::operator*(const float s, const Mat3& mat3) {
	Mat3 prod;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			prod.m[l][c] = mat3.m[l][c] * s;
		}
	}
	return prod;
}

Mat3 LMath::Mat3::operator+(const float s) const {
	Mat3 sum;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			sum.m[l][c] = m[l][c] + s;
		}
	}
	return sum;
}
Mat3 LMath::operator+(const float s, const Mat3& mat3) {
	Mat3 sum;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			sum.m[l][c] = s + mat3.m[l][c];
		}
	}
	return sum;
}
Mat3 LMath::Mat3::operator-(const float s) const {
	Mat3 diff;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			diff.m[l][c] = m[l][c] - s;
		}
	}
	return diff;
}
Mat3 LMath::operator-(const float s, const Mat3& mat3) {
	Mat3 diff;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			diff.m[l][c] = s - mat3.m[l][c];
		}
	}
	return diff;
}
Mat3 LMath::Mat3::operator/(const float s) const {
	Mat3 divid;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			divid.m[l][c] = m[l][c] / s;
		}
	}
	return divid;
}

Vec3 LMath::Mat3::operator*(const Vec3& v) const {
	Vec3 prod;
	prod.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
	prod.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
	prod.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
	return prod;
}

float* LMath::Mat3::operator[](const int lines) {
	return m[lines];
}

Mat3 LMath::Mat3::transpose() const {
	Mat3 trans;
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			trans.m[l][c] = m[c][l];
		}
	}
	return trans;
}

bool LMath::Mat3::inverse(Mat3& mat3) const {
	Mat3 adj;
	Mat3 trans = this->transpose();
	float det = this->determinant();
	
	if (cmpf(det, 0.0f)) return false;

	adj.m[0][0] =	 (trans.m[1][1] * trans.m[2][2] - trans.m[1][2] * trans.m[2][1]);
	adj.m[0][1] = -1*(trans.m[1][0] * trans.m[2][2] - trans.m[1][2] * trans.m[2][0]);
	adj.m[0][2] =	 (trans.m[1][0] * trans.m[2][1] - trans.m[1][1] * trans.m[2][0]);
	adj.m[1][0] = -1*(trans.m[0][1] * trans.m[2][2] - trans.m[0][2] * trans.m[2][1]);
	adj.m[1][1] =	 (trans.m[0][0] * trans.m[2][2] - trans.m[0][2] * trans.m[2][0]);
	adj.m[1][2] = -1*(trans.m[0][0] * trans.m[2][1] - trans.m[0][1] * trans.m[2][0]);
	adj.m[2][0] =	 (trans.m[0][1] * trans.m[1][2] - trans.m[0][2] * trans.m[1][1]);
	adj.m[2][1] = -1*(trans.m[0][0] * trans.m[1][2] - trans.m[0][2] * trans.m[1][0]);
	adj.m[2][2] =	 (trans.m[0][0] * trans.m[1][1] - trans.m[0][1] * trans.m[1][0]);

	mat3 = (1 / det) * adj;
	
	return true;
}

void LMath::Mat3::toOpenGLFormat(float array[9]) const {
	int i = 0;
	for (int c = 0; c < 3; c++) {
		for (int l = 0; l < 3; l++) {
			array[i] = m[l][c];
			++i;
		}
	}
}

bool LMath::Mat3::isOrthogonal() const
{
	return *this * this->transpose() == Mat3::IDENTITY;
}

/*
 * Print result example:
 * [ 1 , 0 , 0 ]
 * [ 0 , 1 , 0 ]
 * [ 0 , 0 , 1 ]
*/
std::ostream& LMath::operator<<(std::ostream& os, const LMath::Mat3& mat3) {
	for (int l = 0; l < 3; l++) {
		os << "[ ";
		for (int c = 0; c < 3; c++) {
			os << mat3.m[l][c];
			if (c < 2) { // Only print comma until the second column
				os << " , ";
			}
		}
		os << " ]" << std::endl;
	}
	return os;
}

std::istream& LMath::operator>>(std::istream& is, LMath::Mat3& mat3) {
	for (int l = 0; l < 3; l++) {
		for (int c = 0; c < 3; c++) {
			is >> mat3.m[l][c];
		}
	}
	return is;
}