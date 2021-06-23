#include "Component.h"

using namespace core;

bool core::Component::setGameObject(GameObject* gameObject)
{
	if (this->gameObject == nullptr) {
		this->gameObject = gameObject;
		return true;
	}
	return false;
}

GameObject* core::Component::getGameObject()
{
	return gameObject;
}
