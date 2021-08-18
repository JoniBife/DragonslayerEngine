#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include "../meshes/Mesh.h"
#include "Material.h"
#include "RenderCommand.h"
#include "PostProcessingCommand.h"

namespace renderer {

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
