#include "Component.h"

using namespace core;

bool Component::setGameObject(GameObject* gameObject)
{
	if (this->gameObject == nullptr) {
		this->gameObject = gameObject;
		return true;
	}
	return false;
}

GameObject* Component::getGameObject()
{
	return gameObject;
}

Component::Component(const std::string& name) : name(name) {}

void core::Component::setElapsedTime(float elapsedTime)
{
	this->elapsedTime = elapsedTime;
}
