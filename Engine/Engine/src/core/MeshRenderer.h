#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../meshes/Mesh.h"
#include "../core/Material.h"
#include "Renderer.h"


namespace core {

	class MeshRenderer : public Renderer {

	private:
		Mesh* mesh;
		Material* material;
	
	public:
		MeshRenderer();

		void onGUI() override;

		void update() override;
	};

};

#endif
