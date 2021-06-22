#define _USE_MATH_DEFINES

#include "MathAux.h"
#include <Math.h>

#define EPSILON 0.00005f

bool cmpf(float A, float B)
{
    return fabsf(A - B) < EPSILON;
}

float round6(float number) {
    return roundf(number * powf(10, 6)) / powf(10, 6);
}

float randomFloat() {
    float a = 5.0;
    return (float(rand()) / float((RAND_MAX))) * a;
}

float randomFloat(float min, float max) {

    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))) + min;
}

float degreesToRadians(float angle) {
    return (angle * float(M_PI)) / 180.0f;
}

float radiansToDegrees(float angleRad) {
    return (angleRad * 180.0f) / float(M_PI);
}

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}