#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

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

	struct PostProcessingCommand {
		
		bool isValid() const;
	};

	class RenderQueue {

	public:
		/* Enqueues a render command, returns false if it fails to enqueue the command, when it is invalid (i.e null mesh or material) */
		virtual bool enqueueRender(RenderCommand renderCommand) = 0;
		/* Enqueues a render command, returns false if it fails to enqueue the command, when it is invalid (i.e null mesh or material) */
		virtual bool enqueuePostProcessing(PostProcessingCommand postProcessingCommand) = 0;
		virtual void clear() = 0;
	};

}

#endif
