#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H


#include <functional>
#include "../math/Mat4.h"

class ICameraController {

public:
	virtual void setOnMovementListener(const std::function<void(Mat4&)>& onMovement) = 0;
	virtual void processInputAndMove(const float elapsedTime) = 0;
};

#endif
