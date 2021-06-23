#include "Component.h"

using namespace core;

bool core::Component::setGameObject(GameObject const* gameObject)
{
	if (this->gameObject == nullptr) {
		this->gameObject = gameObject;
		return true;
	}
		
	return false;
}
