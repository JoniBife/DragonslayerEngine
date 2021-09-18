#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "../meshes/Mesh.h"
#include "../core/Material.h"
#include "RendererComponent.h"

namespace core {

	class MeshRenderer : public RendererComponent {

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

		void onGUI(GUI& gui) override;

		void onStart() override;
		void onFrameUpdate() override;
		void onEnd() override;

		//static MeshRenderer* cube();

		virtual inline bool isSingular() const override;
	};

}

#endif
