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

void core::Hierarchy::updateScene()
{
	for (GameObject* gameObject : rootGameObjects) {
		updateSceneRecursively(gameObject);
	}
}

std::list<core::GameObject*> core::Hierarchy::getRootGameObjects() const
{
	return rootGameObjects;
}

void core::Hierarchy::updateSceneRecursively(GameObject* gameObject, const Mat4& parentModel)
{
	// Update the position with the parent model matrix
	gameObject->transform->update(parentModel);

	// Then update each of the components
	for (Component* component : gameObject->components) {
		component->update();
	}

	// First we update the children
	for (GameObject* child : gameObject->getChildren()) {
		updateSceneRecursively(child, gameObject->transform->getModelMatrix());
	}

	
}

core::Hierarchy& core::Hierarchy::getHierarchy()
{
	assert(instance != nullptr); return *instance;
}

