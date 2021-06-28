#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <assert.h>
#include <list>

namespace core {

	class GameObject;

	class Hierarchy {

	private:
		std::list<GameObject*> rootGameObjects;
		static Hierarchy* instance;

		bool addRoot(GameObject* gameObject);
		bool removeRoot(GameObject* gameObject);

		friend class GameObject; // Transform is a friend class to have access to addRoot and removeRoot

	public:
		static Hierarchy& createHierarchy();
		static Hierarchy& getHierarchy();

		std::list<GameObject*> getRootGameObjects() const;

	};

}

#include "GameObject.h"

#endif
