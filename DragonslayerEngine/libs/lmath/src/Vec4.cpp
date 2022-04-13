#include <math.h>
#include <assert.h>
#include "LMath\Vec4.hpp"
#include "LMath\MathAux.hpp"

using namespace LMath;

LMath::Vec4::Vec4() : Vec4(0) {}
LMath::Vec4::Vec4(float xyz) : Vec4(xyz,xyz,xyz,xyz) {}
LMath::Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

Vec4 LMath::Vec4::ZERO = Vec4(0, 0, 0, 0);
Vec4 LMath::Vec4::Z = Vec4(0, 0, 1, 1);
Vec4 LMath::Vec4::Y = Vec4(0, 1, 0, 1);
Vec4 LMath::Vec4::X = Vec4(1, 0, 0, 1);

Vec4 LMath::Vec4::operator+(const Vec4& other) const {
	return Vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
}

Vec4 LMath::Vec4::operator-(const Vec4& other) const {
	return Vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

Vec4& LMath::Vec4::operator=(const Vec4& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
	return *this;
}

Vec4& LMath::Vec4::operator+=(const Vec4& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
	return *this;
}

Vec4& LMath::Vec4::operator-=(const Vec4& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
	return *this;
}



Vec4 LMath::Vec4::operator*(float scalar) const {
	return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

Vec4 LMath::Vec4::operator/(float scalar) const {
	return Vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
}

Vec4 LMath::Vec4::operator+(float scalar) const {
	return Vec4(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
}

Vec4 LMath::Vec4::operator-(float scalar) const {
	return Vec4(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
}

Vec4& LMath::Vec4::operator+=(float s) {
	this->x += s;
	this->y += s;
	this->z += s;
	this->w += s;
	return *this;
}

Vec4& LMath::Vec4::operator-=(float s) {
	this->x -= s;
	this->y -= s;
	this->z -= s;
	this->w -= s;
	return *this;
}

Vec4& LMath::Vec4::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

Vec4& LMath::Vec4::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	this->w /= s;
	return *this;
}



bool LMath::Vec4::operator==(const Vec4& other) const {
	return cmpf(this->x, other.x) && cmpf(this->y, other.y) && cmpf(this->z, other.z) && cmpf(this->w, other.w);
}

bool LMath::Vec4::operator!=(const Vec4& other) const {
	return !(*this == other);
}



float LMath::Vec4::magnitude() const {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w*this->w);
}

float LMath::Vec4::sqrMagnitude() const {
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w* this->w;
}

Vec4 LMath::Vec4::normalize() const {
	float magnitude = this->magnitude();

	// Cannot divide by 0
	assert(magnitude > 0);

	return (*this) / magnitude;
}

void LMath::Vec4::toOpenGLFormat(float array[4]) const {
	array[0] = this->x;
	array[1] = this->y;
	array[2] = this->z;
	array[3] = this->w;
}

float LMath::dot(const Vec4& a, const Vec4& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

std::ostream& LMath::operator<<(std::ostream& os, const Vec4& vec4) {
	return os << "(" << vec4.x << "," << vec4.y << "," << vec4.z << "," << vec4.w << ')';
}

Vec4 LMath::operator*(float scalar, const Vec4& vec4) {
	return Vec4(scalar * vec4.x, scalar * vec4.y, scalar * vec4.z, scalar * vec4.w);
}

Vec4 LMath::operator+(float scalar, const Vec4& vec4) {
	return Vec4(scalar + vec4.x, scalar + vec4.y, scalar + vec4.z, scalar + vec4.w);
}

Vec4 LMath::operator-(float scalar, const Vec4& vec4) {
	return Vec4(scalar - vec4.x, scalar - vec4.y, scalar - vec4.z, scalar - vec4.w);
}