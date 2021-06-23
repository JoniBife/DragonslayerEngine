#include "GameObject.h"

#include <type_traits>

using namespace core;

GameObject::GameObject(const std::string& name) : transform(new Transform()), name(name) {}

GameObject::~GameObject() {
	delete transform;

	for (Component* component : components)
		delete component;
}

std::string core::GameObject::getName() const
{
	return name;
}

Transform& core::GameObject::getTransform() const
{
	return *transform;
}

bool core::GameObject::addComponent(Component* component)
{
	if (std::find(components.begin(), components.end(), component) == components.end()
		&& component->setGameObject(this)) {
		components.push_back(component);
		return true;
	}
	return false;
}