#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include "../meshes/Mesh.h"
#include "Material.h"

namespace renderer {

	struct RenderCommand {
		Mesh* mesh;
		Material* material;
		Mat4 model;
		bool castShadows;
		bool receiveShadows;
		bool blending;

		/* Check if the rendering command is valid (i.e. Mesh is not null nor material) */
		bool isValid() const;
	};

}

#endif
