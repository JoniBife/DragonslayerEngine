#include "Hierarchy.h"

using namespace core;

bool Hierarchy::addRoot(GameObject* gameObject)
{
	for (GameObject* root : rootGameObjects) {
		if (root == gameObject)
			return false;
	}
	
	rootGameObjects.push_back(gameObject);
	return true;
}

bool Hierarchy::removeRoot(GameObject* gameObject)
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

Hierarchy* Hierarchy::instance = new Hierarchy();

Hierarchy& Hierarchy::createHierarchy() {
	// There can only be one instance of hierachy
	assert(instance == nullptr);
	//instance = 
	return *instance;
}

void Hierarchy::updateScene()
{
	for (GameObject* gameObject : rootGameObjects) {
		updateSceneRecursively(gameObject);
	}
}

std::list<GameObject*> Hierarchy::getRootGameObjects() const
{
	return rootGameObjects;
}

void Hierarchy::traverseScene(const std::function<void(GameObject*)>& onGameObject)
{
	for (GameObject* root : rootGameObjects) {
		traverseSceneRecursively(root, onGameObject);
	}
}

void Hierarchy::traverseSceneRecursively(GameObject* gameObject, const std::function<void(GameObject*)>& onGameObject)
{
	onGameObject(gameObject);

	for (GameObject* child : gameObject->getChildren()) {
		traverseSceneRecursively(child,	onGameObject);
	}
}

void Hierarchy::updateSceneRecursively(GameObject* gameObject, const Mat4& parentModel)
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

Hierarchy& Hierarchy::getHierarchy()
{
	assert(instance != nullptr); return *instance;
}

