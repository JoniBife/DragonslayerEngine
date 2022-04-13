#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include "Vec4.hpp"
#include "Mat4.hpp"

namespace LMath {

	struct Qtrn {

	public:
		float t, x, y, z;

		void clean();

		Qtrn();
		Qtrn(const float thetaRad, const Vec3& axis);
		Qtrn(const float t, const float x, const float y, const float z);

		void toAngleAxis(float& thetaRad, Vec3& axis) const;

		float quadrance() const;
		float norm() const;

		Qtrn normalize() const;
		Qtrn conjugate() const;
		Qtrn inverse() const;

		Qtrn operator*(const float s) const;
		friend Qtrn operator*(const float s, const Qtrn& q);
		Qtrn operator*(const Qtrn& q) const;
		Qtrn operator+(const Qtrn& q) const;
		Qtrn& operator=(const Qtrn& q);

		Qtrn& operator*=(const float s);
		Qtrn& operator*=(const Qtrn& q);
		Qtrn& operator+=(const Qtrn& q);

		bool operator==(const Qtrn& q) const;
		bool operator!=(const Qtrn& q) const;

		static Qtrn lerp(const Qtrn& q0, const Qtrn& q1, float k);
		static Qtrn slerp(const Qtrn& q0, const Qtrn& q1, float k);
		static Qtrn fromDir(const Vec3& dir, const Vec3& ref = { 0.0f, 1.0f, 0.0f });

		Mat4 toRotationMatrix() const;

		friend std::ostream& operator<<(std::ostream& os, const Qtrn& qtrn);
		void printAngleAxis();
	};

}

#endif
