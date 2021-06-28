#include "GameObject.h"

#include <type_traits>

using namespace core;

GameObject::GameObject(const std::string& name) : transform(new Transform()), name(name) , hierarchy(Hierarchy::getHierarchy()) {
	// When a gameobject is created it gets added automatically to the hierarchy
	hierarchy.addRoot(this);
}

GameObject::~GameObject() {

	// No parent, so it is a root and we have to remove it from the hierarchy
	if (parent == nullptr) {
		hierarchy.removeRoot(this);
	}

	// Delete children
	for (GameObject* child : children) {
		delete child;
	}

	// Delete components
	delete transform;
	for (Component* component : components)
		delete component;
}

std::string core::GameObject::getName() const
{
	return name;
}

Transform* core::GameObject::getTransform() const
{
	return transform;
}

bool core::GameObject::addComponent(Component* component)
{
	assert(component != nullptr);

	if (std::find(components.begin(), components.end(), component) == components.end()
		&& component->setGameObject(this)) {
		components.push_back(component);
		return true;
	}
	return false;
}

bool core::GameObject::addChildren(GameObject* gameObject) {

	// Cannot add null children
	assert(gameObject != nullptr);

	if (gameObject == this)
		return false;

	for (GameObject* child : children) {
		if (child == gameObject)
			return false;
	}

	gameObject->setParent(this);

	children.push_back(gameObject);
	return true;
}

std::list<GameObject*> core::GameObject::getChildren() const {
	return children;
}
GameObject* core::GameObject::getParent() const {
	return parent;
}

unsigned int core::GameObject::numberOfChildren() const {
	return children.size();
}

void core::GameObject::setParent(GameObject* gameObject)
{
	assert(gameObject != this);

	if (parent == nullptr) {
		hierarchy.removeRoot(this);
	}

	parent = gameObject;
}

void core::GameObject::select()
{
	selected = true;
}

void core::GameObject::unselect()
{
	selected = false;
}

bool core::GameObject::isSelected() const {
	return selected;
}