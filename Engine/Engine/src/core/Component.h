#ifndef COMPONENT_H
#define COMPONENT_H

namespace core {

	class GameObject; // Forward declaration of Component due to cross referencing between Component and GameObject

	/*
	* Base class for all components. A component can be added to a GameObject.
	* Examples of components include: RigidBody, MeshRenderer etc..
	*/
	class Component {

	public:
		virtual bool getType() const = 0;

		/*
		* Associates GameObject with this components
		* if the component already has a GameObject associated
		* then return false else return true
		*/
		bool setGameObject(GameObject* gameObject);

		GameObject* getGameObject();

	private:
		GameObject* gameObject;

	};

};

#include "GameObject.h"

#endif 


