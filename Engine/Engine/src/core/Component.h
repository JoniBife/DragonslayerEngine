#ifndef COMPONENT_H
#define COMPONENT_H

#include "../gui/GUI.h"
#include <string>

namespace core {

	class GameObject; // Forward declaration of Component due to cross referencing between Component and GameObject

	/* Base class for all components. A component can be added to a GameObject.
	* Examples of components include: RigidBody, MeshRenderer etc..
	*/
	class Component {

	public:
		/* Indicates whether the game object can contain a single component of this type*/
		virtual inline bool isSingular() const = 0;
		virtual bool getType() const = 0;
		std::string getName() const { return name; }
		float getElapsedTime() const { return elapsedTime; }

		/*! @brief Associates GameObject with this components
		* if the component already has a GameObject associated
		* then return false else return true
		*/
		bool setGameObject(GameObject* gameObject);

		GameObject* getGameObject();

		/* Called by the GUI to display the component */
		virtual void onGUI() = 0;

	protected:
		Component(const std::string& name);

		GameObject* gameObject = nullptr;

	private:
		std::string name;
		float elapsedTime;

		void setElapsedTime(float elapsedTime);

		virtual void onStart() = 0;

		/* Called by the scene graph to update the component every frame */
		virtual void onFrameUpdate() = 0;

		virtual void onEnd() = 0;

		friend class Hierarchy;
	};

}

#endif 


