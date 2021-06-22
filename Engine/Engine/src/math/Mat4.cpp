#include <math.h>
#include "Mat4.h"
#include "MathAux.h"

Mat4::Mat4() : Mat4(0) {}

Mat4::Mat4(const float fill) : Mat4(fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill) {}

Mat4::Mat4(const float l1c1, const float l1c2, const float l1c3, const float l1c4,
	const float l2c1, const float l2c2, const float l2c3, const float l2c4,
	const float l3c1, const float l3c2, const float l3c3, const float l3c4,
	const float l4c1, const float l4c2, const float l4c3, const float l4c4) {

	m[0][0] = l1c1;
	m[0][1] = l1c2;
	m[0][2] = l1c3;
	m[0][3] = l1c4;
	m[1][0] = l2c1;
	m[1][1] = l2c2;
	m[1][2] = l2c3;
	m[1][3] = l2c4;
	m[2][0] = l3c1;
	m[2][1] = l3c2;
	m[2][2] = l3c3;
	m[2][3] = l3c4;
	m[3][0] = l4c1;
	m[3][1] = l4c2;
	m[3][2] = l4c3;
	m[3][3] = l4c4;
}

Mat4::Mat4(const Mat4& other) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] = other.m[l][c];
		}
	}
}

Mat4 Mat4::ZERO = { 0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0 };

Mat4 Mat4::IDENTITY = {1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1};

Mat4 Mat4::scaling(const float xyz)
{
	return scaling(xyz, xyz, xyz);
}

Mat4 Mat4::scaling(const float x, const float y, const float z)
{
	return {x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1};
}

Mat4 Mat4::scaling(const Vec3& v)
{
	return { v.x, 0, 0, 0,
			0, v.y, 0, 0,
			0, 0, v.z, 0,
			0, 0, 0, 1 };
}

Mat4 Mat4::translation(const float x, const float y, const float z)
{
	return {1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1};
}

Mat4 Mat4::translation(const Vec3& v)
{
	return { 1, 0, 0, v.x,
			0, 1, 0, v.y,
			0, 0, 1, v.z,
			0, 0, 0, 1 };
}

Mat4 Mat4::rotation(const float angleRad, const Vec3& axis)
{
	Vec3 a = axis.normalize();

	Mat4 dualMatrix = {
			0  ,-a.z,  a.y, 0,
		 a.z,   0   , -a.x, 0,
		-a.y, a.x,    0   , 0,
			0  ,   0   ,    0   , 0 // Zero so that we don't have to set the last element of the matrix to 1
	};
	Mat4 dualMatrixSqr = {
			-(a.y*a.y) - a.z * a.z, a.x*a.y, a.x*a.z, 0,
			 a.x* a.y, -(a.x*a.x)-(a.z*a.z), a.y*a.z, 0,
			 a.x* a.z, a.y* a.z, -(a.x * a.x)-(a.y * a.y), 0,
					0      ,		0		,				0					, 0 // Zero so that we don't have to set the last element of the matrix to 1
	};

	Mat4 identity = { 1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1 };

	return identity + sinf(angleRad) * dualMatrix + (1 - cosf(angleRad)) * dualMatrixSqr;
}

Mat4 Mat4::rotationFromDir(const Vec3& dir, const Vec3& up) {
	Vec3 xaxis = cross(up, dir).normalize();
	Vec3 yaxis = cross(dir, xaxis).normalize();

	if (xaxis == Vec3::ZERO && yaxis == Vec3::ZERO)
		return Mat4::IDENTITY;
	Mat4 rotation;
	rotation[0][0] = xaxis.x;
	rotation[0][1] = yaxis.x;
	rotation[0][2] = dir.x;
	rotation[0][3] = 0.0f;

	rotation[1][0] = xaxis.y;
	rotation[1][1] = yaxis.y;
	rotation[1][2] = dir.y;
	rotation[1][3] = 0.0f;

	rotation[2][0] = xaxis.z;
	rotation[2][1] = yaxis.z;
	rotation[2][2] = dir.z;
	rotation[2][3] = 0.0f;

	rotation[3][0] = 0.0f;
	rotation[3][1] = 0.0f;
	rotation[3][2] = 0.0f;
	rotation[3][3] = 1.0f;

	return rotation;
}

Mat4& Mat4::operator=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] = other.m[l][c];			
		}
	}
	return *this;
}

Mat4& Mat4::operator+=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] += other.m[l][c];
		}
	}
	return *this;
}

Mat4& Mat4::operator-=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] -= other.m[l][c];
		}
	}
	return *this;
}

Mat4& Mat4::operator*=(const Mat4& other)
{
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = m[l][0]*other.m[0][c] + 
					  m[l][1]*other.m[1][c] + 
					  m[l][2]*other.m[2][c] + 
					  m[l][3]*other.m[3][c];
		}
	}
	*this = prod;
	return *this;
}

Mat4& Mat4::operator*=(const float s) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] *= s;
		}
	}
	return *this;
}

Mat4& Mat4::operator/=(const float s) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] /= s;
		}
	}
	return *this;
}

Mat4& Mat4::operator+=(const float s) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] += s;
		}
	}
	return *this;
}

Mat4& Mat4::operator-=(const float s) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] -= s;
		}
	}
	return *this;
}

bool Mat4::operator==(const Mat4& other) const {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			if (!cmpf(m[l][c], other.m[l][c]))
				return false;
		}
	}
	return true;
}

bool Mat4::operator!=(const Mat4& other) const {
	return !(*this == other);
}

Mat4 Mat4::operator+(const Mat4& other) const {
	Mat4 sum;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			sum.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return sum;
}
Mat4 Mat4::operator-(const Mat4& other) const {
	Mat4 diff;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			diff.m[l][c] = m[l][c] - other.m[l][c];
		}
	}
	return diff;
}
Mat4 Mat4::operator*(const Mat4& other) const {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = m[l][0] * other.m[0][c] + 
						   m[l][1] * other.m[1][c] + 
						   m[l][2] * other.m[2][c] + 
						   m[l][3] * other.m[3][c];
		}
	}
	return prod;
}
Mat4 Mat4::operator*(const float s) const {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = m[l][c] * s;
		}
	}
	return prod;
}


Mat4 operator*(const float s, const Mat4& mat4) {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = mat4.m[l][c] * s;
		}
	}
	return prod;
}

Mat4 Mat4::operator+(const float s) const {
	Mat4 sum;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			sum.m[l][c] = m[l][c] + s;
		}
	}
	return sum;
}
Mat4 operator+(const float s, const Mat4& mat4) {
	Mat4 sum;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			sum.m[l][c] = s + mat4.m[l][c];
		}
	}
	return sum;
}
Mat4 Mat4::operator-(const float s) const {
	Mat4 diff;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			diff.m[l][c] = m[l][c] - s;
		}
	}
	return diff;
}
Mat4 operator-(const float s, const Mat4& mat4) {
	Mat4 diff;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			diff.m[l][c] = s - mat4.m[l][c];
		}
	}
	return diff;
}
Mat4 Mat4::operator/(const float s) const {
	Mat4 divid;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			divid.m[l][c] = m[l][c] / s;
		}
	}
	return divid;
}

Vec4 Mat4::operator*(const Vec4& v) const {
	Vec4 prod;
	prod.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	prod.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	prod.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	prod.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return prod;
}

float* Mat4::operator[](const int lines) {
	return m[lines];
}

Mat4 Mat4::transpose() const {
	Mat4 trans;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			trans.m[l][c] = m[c][l];
		}
	}
	return trans;
}

void Mat4::toOpenGLFormat(float array[16]) const {
	int i = 0;
	for (int c = 0; c < 4; c++) {
		for (int l = 0; l < 4; l++) {
			array[i] = m[l][c];
			++i;
		}
	}
}

Mat3 Mat4::toMat3() const {

	return { m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2] };
}

/*
 * Print result example:
 * [ 1 , 0 , 0 , 0 ]
 * [ 0 , 1 , 0 , 0 ]
 * [ 0 , 0 , 1 , 0 ]
 * [ 0 , 0 , 0 , 1 ]
*/
std::ostream& operator<<(std::ostream& os, const Mat4& mat4) {
	for (int l = 0; l < 4; l++) {
		os << "[ ";
		for (int c = 0; c < 4; c++) {
			os << mat4.m[l][c];
			if (c < 3) { // Only print comma until the third column
				os << " , ";
			}
		}
		os << " ]" << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, Mat4& mat4) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			is >> mat4.m[l][c];
		}
	}
	return is;
}