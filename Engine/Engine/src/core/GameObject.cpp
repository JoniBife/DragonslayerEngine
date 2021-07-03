#include "GameObject.h"

#include <type_traits>
#include "Component.h"
#include "Hierarchy.h"

using namespace core;

GameObject::GameObject(const std::string& name) : transform(new Transform()), name(name), hierarchy(Hierarchy::getHierarchy()) {
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

std::string GameObject::getName() const
{
	return name;
}

Transform* GameObject::getTransform() const
{
	return transform;
}

bool GameObject::addComponent(Component* component)
{
	assert(component != nullptr);

	if (component->getGameObject() != nullptr)
		return false;

	if (std::find(components.begin(), components.end(), component) == components.end()
		&& component->setGameObject(this)) {
		components.push_back(component);
		return true;
	}
	return false;
}

bool GameObject::addChildren(GameObject* gameObject) {

	// Cannot add null children
	assert(gameObject != nullptr);

	if (gameObject == this)
		return false;

	for (GameObject* child : children) {
		if (child == gameObject)
			return false;
	}

	gameObject->setParent(this);
	//gameObject->transform->makeChildOfTransform(transform);

	children.push_back(gameObject);
	return true;
}

std::list<GameObject*> GameObject::getChildren() const {
	return children;
}
GameObject* GameObject::getParent() const {
	return parent;
}

unsigned int GameObject::numberOfChildren() const {
	return children.size();
}

std::vector<Component*> GameObject::getAttachedComponents() const
{
	return components;
}

void GameObject::setParent(GameObject* gameObject)
{
	assert(gameObject != this);

	if (parent == nullptr && gameObject != nullptr) {
		hierarchy.removeRoot(this);
	}
	else if (parent != nullptr && gameObject == nullptr) {
		hierarchy.addRoot(this);
	}

	parent = gameObject;
}

void GameObject::select()
{
	selected = true;
}

void GameObject::unselect()
{
	selected = false;
}

bool GameObject::isSelected() const {
	return selected;
}