#include "LMath\Mat2.hpp"
#include "LMath\MathAux.hpp"

using namespace LMath;

LMath::Mat2::Mat2() : Mat2(0) {}
LMath::Mat2::Mat2(const float fill) : Mat2(fill,fill,fill,fill) {}
LMath::Mat2::Mat2(const float l1c1, const float l1c2, const float l2c1, const float l2c2)
{
	m[0][0] = l1c1;
	m[0][1] = l1c2;
	m[1][0] = l2c1;
	m[1][1] = l2c2;
}

LMath::Mat2::Mat2(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] = other.m[l][c];
		}
	}
}

Mat2 LMath::Mat2::IDENTITY = {1, 0,
						0, 1};

Mat2& LMath::Mat2::operator=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] = other.m[l][c];
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator+=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] += other.m[l][c];
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator-=(const Mat2& other) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] -= other.m[l][c];
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator*=(const Mat2& other) {
	// We have to put the result in another variable 
	// otherwise we will be calculating the multiplication with the result at the same time
	Mat2 prod; 
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = m[l][0] * other.m[0][c] + m[l][1] * other.m[1][c];
		}
	}
	*this = prod;
	return *this;
}

Mat2& LMath::Mat2::operator*=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] *= s;
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator/=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] /= s;
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator+=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] += s;
		}
	}
	return *this;
}

Mat2& LMath::Mat2::operator-=(const float s) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			m[l][c] -= s;
		}
	}
	return *this;
}



bool LMath::Mat2::operator==(const LMath::Mat2& other) const {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			if (!cmpf(m[l][c], other.m[l][c]))
				return false;
		}
	}
	return true;
}

bool LMath::Mat2::operator!=(const LMath::Mat2& other) const {
	return !(*this == other);
}

Mat2 LMath::Mat2::operator+(const Mat2& other) const {
	Mat2 sum;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			sum.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return sum;
}

Mat2 LMath::Mat2::operator-(const Mat2& other) const {
	Mat2 diff;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			diff.m[l][c] = m[l][c] - other.m[l][c];
		}
	}
	return diff;
}
Mat2 LMath::Mat2::operator*(const Mat2& other) const {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = m[l][0] * other.m[0][c] + m[l][1] * other.m[1][c];
		}
	}
	return prod;
}
Mat2 LMath::Mat2::operator*(const float s) const {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = m[l][c] * s;
		}
	}
	return prod;
}

Mat2 LMath::Mat2::operator+(const float s) const {
	Mat2 sum;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			sum.m[l][c] = m[l][c] + s;
		}
	}
	return sum;
}

Mat2 LMath::operator+(const float s, const Mat2& mat2) {
	Mat2 sum;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			sum.m[l][c] = s + mat2.m[l][c];
		}
	}
	return sum;
}
Mat2 LMath::Mat2::operator-(const float s) const {
	Mat2 diff;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			diff.m[l][c] = m[l][c] - s;
		}
	}
	return diff;
}

Mat2 LMath::operator-(const float s, const Mat2& mat2) {
	Mat2 diff;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			diff.m[l][c] = s - mat2.m[l][c];
		}
	}
	return diff;
}

Mat2 LMath::Mat2::operator/(const float s) const {
	Mat2 divid;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			divid.m[l][c] = m[l][c] / s;
		}
	}
	return *this;
}


Mat2 LMath::operator*(const float s, const Mat2& mat2) {
	Mat2 prod;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			prod.m[l][c] = mat2.m[l][c] * s;
		}
	}
	return prod;
}
Vec2 LMath::Mat2::operator*(const Vec2& v) const {
	Vec2 prod;
	prod.x = m[0][0] * v.x + m[0][1] * v.y;
	prod.y = m[1][0] * v.x + m[1][1] * v.y;
	return prod;
}

float* LMath::Mat2::operator[](const int lines) {
	return m[lines];
}

Mat2 LMath::Mat2::transpose() const {
	Mat2 trans;
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			trans.m[l][c] = m[c][l];
		}
	}
	return trans;
}
bool LMath::Mat2::inverse(Mat2& inverse) const {
	float det = determinant();

	if (cmpf(det, 0.0f)) // If the determinant is 0 the matrix is not invertible
		return false;

	inverse = (1.0f / det) * Mat2(m[1][1], -m[0][1],
								  -m[1][0], m[0][0]);
	return true;
}
float LMath::Mat2::determinant() const {
	return m[0][0]*m[1][1] - m[0][1]*m[1][0];
}
void LMath::Mat2::toOpenGLFormat(float array[4]) const {
	int i = 0;
	for (int c = 0; c < 2; c++) {
		for (int l = 0; l < 2; l++) {
			array[i] = m[l][c];
			++i;
		}
	}
}

bool LMath::Mat2::isOrthogonal() const
{
	return *this * this->transpose() == Mat2::IDENTITY;
}

std::ostream& LMath::operator<<(std::ostream& os, const LMath::Mat2& mat2) {
	for (int l = 0; l < 2; l++) {
		os << "[ ";
		for (int c = 0; c < 2; c++) {
			os << mat2.m[l][c];
			if (c < 1) { // Only print on the first column
				os << " , ";
			}
		}
		os << " ]" << std::endl;
	}
	return os;
}
std::istream& LMath::operator>>(std::istream& is, LMath::Mat2& mat2) {
	for (int l = 0; l < 2; l++) {
		for (int c = 0; c < 2; c++) {
			is >> mat2.m[l][c];
		}
	}
	return is;
}
