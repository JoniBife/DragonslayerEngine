#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../meshes/Mesh.h"
#include "../core/Material.h"
#include "Renderer.h"

namespace core {

	class MeshRenderer : public Renderer {

	private:
		Mesh* mesh = nullptr;
		Material* material = nullptr;
	
	public:
		MeshRenderer();
		MeshRenderer(Mesh* mesh, Material* material);

		void setMesh(Mesh* mesh);
		void setMaterial(Material* material);

		Mesh* getMesh() const;
		Material* getMaterial() const;

		void onGUI() override;

		void update() override;
	};

};

#endif
