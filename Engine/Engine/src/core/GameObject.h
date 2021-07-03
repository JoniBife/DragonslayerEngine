#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Transform.h"

namespace core {

	class Hierarchy;

	class Component; // Forward declaration of Component due to cross referencing between Component and GameObject

	class GameObject {

	private:
		// Scene graph related fields
		Hierarchy& hierarchy;
		GameObject* parent = nullptr;
		std::list<GameObject*> children;
		bool selected = false;

		// Component related fields
		Transform* transform = nullptr;
		std::vector<Component*> components;

		// Identification related fields
		std::string name = "";
		unsigned long long uniqueId = 0; // TODO Implement
		
	public:

		GameObject(const std::string& name = "GameObject");
		~GameObject();

		std::string getName() const;
		Transform* getTransform() const;

		/* Adds component to the components vector
		* If component already has game object associated
		* return false else return true
		* TODO Maybe Check if the component allows for duplicates
		*/
		bool addComponent(Component* component);

		/* Returns the first component of type T
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
		
		/* Returns a vector containing all components of type T
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

		std::vector<Component*> getAttachedComponents() const;

		/* Removes the component if it is attached to this gameobject*/
		bool removeComponent(Component* component);

		//////////////SCENE GRAPH/////////////////////

		/* Adds child to GameObject, returns false if it is already a child else true */
		bool addChildren(GameObject* gameObject);


		std::list<GameObject*> getChildren() const;
		GameObject* getParent() const;
		void setParent(GameObject* gameObject);

		unsigned int numberOfChildren() const;

		void select();
		void unselect();
		bool isSelected() const;

		///////////////////////////////////////////////

		friend class Hierarchy;
	};
}

#endif