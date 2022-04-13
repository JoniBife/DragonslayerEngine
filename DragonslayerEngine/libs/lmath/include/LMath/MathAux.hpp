#ifndef MATHAUX_H
#define MATHAUX_H

#include <iostream>
#include <math.h>
#include "Vec3.hpp"

namespace LMath {

#define PI 3.141592741f

#define SQR(x) ((x) * (x))

    /*
    *
    * Checks if the floats A and B are equal
    * using FLT_EPISILON as error margin
    *
    * @param A first float
    * @param B second float
    *
    * @return true if A equals B else false
    */
    bool cmpf(float A, float B);

    float round6(float number);

    float randomFloat();

    float randomFloat(float min, float max);

    float degreesToRadians(float angle);

    float radiansToDegrees(float angleRad);

    float lerp(float a, float b, float f);

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

#define signf sgn<float>
#define fsignf sgn<double>
#define sign sgn<int>

}

#endif