#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "../math/Mat4.h"
#include "../math/Vec3.h"

Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);

/* Ignores the near / far transformation, the view frustrum is only limited by the left, right, bottom and top */
Mat4 ortho(float left, float right, float bottom, float top);

Mat4 ortho(float left, float right, float bottom, float top, float near, float far);

Mat4 perspective(float fovy, float aspectRatio, float near, float far);

/* Creates an orthographic matrix from a cascade, defined by the near and far planes passed as parameters, within the camera view frustrum  */
Mat4 orthoCascade(float nearViewSpace, float farViewSpace, float fovRad, float inverseAspectRatio, Mat4 inverseCameraView, Mat4 lightView);

#endif