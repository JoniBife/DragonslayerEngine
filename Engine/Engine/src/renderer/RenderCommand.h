#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

#include "../meshes/Mesh.h"
#include "PMaterial.h"

namespace WarriorRenderer {

	struct RenderCommand {
		Mesh* mesh;
		PMaterial* material;
		Mat4 model;
		bool castShadows = true;
		bool receiveShadows = true;
		bool blending;

		/* Check if the rendering command is valid (i.e. Mesh is not null nor material) */
		bool isValid() const;
	};

}

#endif
