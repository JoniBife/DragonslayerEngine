#ifndef COMPONENT_H
#define COMPONENT_H

namespace core {

	class GameObject;

	/*
	* Base class for all components. A component can be added to a GameObject.
	* Examples of components include: RigidBody, MeshRenderer etc..
	*/
	class Component {

	public:
		/*
		* Associates GameObject with this components
		* if the component already has a GameObject associated
		* then return false else return true
		*/
		bool setGameObject(GameObject const* gameObject);

	private:
		GameObject const* gameObject;

	};

};

#include "GameObject.h"

#endif 


