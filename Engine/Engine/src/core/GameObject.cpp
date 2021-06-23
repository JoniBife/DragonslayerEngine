#include "GameObject.h"

using namespace core;

GameObject::GameObject(const std::string& name) : transform(new Transform()), name(name) {}

GameObject::~GameObject() {
	delete transform;

	// Does not delete components because they are managed elsewhere TODO improve commment
}

std::string core::GameObject::getName() const
{
	return name;
}

Transform& core::GameObject::getTransform() const
{
	return *transform;
}

void core::GameObject::addComponent(Component const* component)
{
	components.push_back(component);
}

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type*>
T const* GameObject::getComponent() const {
	for (Component* component : components) {

		if (dynamic_cast<T*>(component) != nullptr) {
			return component;
		}
	}
	return nullptr;
}

template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type*>
std::vector<T const*> GameObject::getComponents() const {
	std::vector<T&> foundComponents;
	for (Component* component : components) {

		if (dynamic_cast<T const*>(component) != nullptr) {
			foundComponents.push_back(component);
		}
	}
	return foundComponents;
}