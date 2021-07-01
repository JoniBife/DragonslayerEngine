#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <assert.h>
#include <list>
#include "../math/Mat4.h"
#include <functional>

namespace core {

	class GameObject;

	class Hierarchy {

	private:
		std::list<GameObject*> rootGameObjects;
		
		bool addRoot(GameObject* gameObject);
		bool removeRoot(GameObject* gameObject);

		void traverseSceneRecursively(GameObject* gameObject, const std::function<void(GameObject*)>& onGameObject);
		void updateSceneRecursively(GameObject* gameObject, const Mat4& parentModel = Mat4::IDENTITY);

		friend class GameObject; // Transform is a friend class to have access to addRoot and removeRoot

	public:
		static Hierarchy& createHierarchy();
		static Hierarchy& getHierarchy();

		GameObject& createGameObject(const std::string& name = "GameObject");
		void deleteGameObject(GameObject& gameObject);

		static Hierarchy* instance;

		std::list<GameObject*> getRootGameObjects() const;

		void traverseScene(const std::function<void(GameObject*)>& onGameObject);

		void updateScene();
	};

}

#include "GameObject.h"

#endif
