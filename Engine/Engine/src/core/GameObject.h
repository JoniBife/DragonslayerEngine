#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Transform.h"

namespace core {

	class Component; // Forward declaration of Component due to cross referencing between Component and GameObject

	class GameObject {

	private:
		Transform* transform = nullptr;
		std::vector<Component*> components;
		std::string name = "";
		unsigned long long uniqueId = 0; // TODO Implement
		
	public:

		GameObject(const std::string& name = "GameObject");
		~GameObject();

		std::string getName() const;
		Transform& getTransform() const;

		/* ! @brief Adds component to the components vector
		* If component already has game object associated
		* return false else return true
		* TODO Maybe Check if the component allows for duplicates
		*/
		bool addComponent(Component* component);

		/*! @brief Returns the first component of type T
		* found in the component list otherwise nullptr
		* 
		* returns * instead of reference because it can be null
		* but its pointer must not be altered
		* 
		* TODO Move declaration to CPP
		*/
		template <typename T, typename = std::enable_if<std::is_base_of<Component, T>::value>>
		T* getComponent() const {
			T* foundComponent;
			for (Component* component : components) {

				if ((foundComponent = dynamic_cast<T*>(component)) != nullptr) {
					return foundComponent;
				}
			}
			return nullptr;
		}
		
		/*! @brief Returns a vector containing all components of type T
		* return * instead of reference for the sake of consistency with the previous method
		* 
		* TODO Move declaration to CPP
		*/
		template <typename T, typename = std::enable_if<std::is_base_of<Component, T>::value>>
		std::vector<T*> getComponents() const {
			std::vector<T*> foundComponents;
			T* foundComponent;
			for (Component* component : components) {

				if ((foundComponent = dynamic_cast<T*>(component)) != nullptr) {
					foundComponents.push_back(foundComponent);
				}
			}
			return foundComponents;
		}
	};

};

#include "Component.h"

#endif