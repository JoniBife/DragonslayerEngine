#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Transform.h"

namespace core {

	class Component;

	class GameObject {

	private:
		Transform* transform;
		std::vector<Component const*> components;
		std::string name;
		unsigned long long uniqueId = 0; // TODO Implement
		
	public:

		GameObject(const std::string& name = "GameObject");
		~GameObject();

		std::string getName() const;
		Transform& getTransform() const;

		/*
		* Adds component to the components vector
		* TODO Maybe Check if the component allows for duplicates
		*/
		void addComponent(Component const* component);

		/*
		* Returns the first component of type T
		* found in the component list otherwise nullptr
		* 
		* returns const * instead of reference because it can be null
		* but its pointer must not be altered
		*/
		template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T const* getComponent() const;
		
		/*
		* Returns a vector containing all components of type T
		* 
		* return const * instead of reference for the sake of consistency with the previous method
		*/
		template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		std::vector<T const*> getComponents() const;
	};

};

#include "Component.h"

#endif