#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Component.h"

class GameObject {

private:
	std::vector<Component> components;

public:
	Component getComponent();
};

#endif