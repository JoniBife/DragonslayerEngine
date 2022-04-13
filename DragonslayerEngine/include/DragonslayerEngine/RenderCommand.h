#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include "DragonslayerEngine/Mesh.h"
#include "Material.h"

namespace DragonslayerEngine {

	struct RenderCommand {
		Mesh* mesh;
		Material* material;
		Mat4 model;
		bool castShadows = true;
		bool receiveShadows = true;
		bool blending;

		/* Check if the rendering command is valid (i.e. Mesh is not null nor material) */
		bool isValid() const;
	};

}

#endif
