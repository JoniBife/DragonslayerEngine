#include "Hierarchy.h"

bool core::Hierarchy::addRoot(GameObject* gameObject)
{
	for (GameObject* root : rootGameObjects) {
		if (root == gameObject)
			return false;
	}
	
	rootGameObjects.push_back(gameObject);
	return true;
}

bool core::Hierarchy::removeRoot(GameObject* gameObject)
{
	bool canRemove = false;
	for (GameObject* root : rootGameObjects) {
		if (root == gameObject) {
			canRemove = true;
			break;
		}
	}

	if (canRemove)
		rootGameObjects.remove(gameObject);

	return canRemove;
}

core::Hierarchy* core::Hierarchy::instance = new Hierarchy();

core::Hierarchy& core::Hierarchy::createHierarchy() {
	// There can only be one instance of hierachy
	assert(instance == nullptr);
	//instance = 
	return *instance;
}

std::list<core::GameObject*> core::Hierarchy::getRootGameObjects() const
{
	return rootGameObjects;
}

core::Hierarchy& core::Hierarchy::getHierarchy()
{
	assert(instance != nullptr); return *instance;
}

