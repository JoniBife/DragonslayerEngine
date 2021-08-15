#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include "../view/Camera.h"
#include "../textures/FrameBuffer.h"
#include "RenderQueue.h"
#include "Lights.h"

namespace renderer {

	/*
	* Base class for any render pipeline (deferred, forward etc..)
	* This way the render is abstracted from this structure and can save multiple
	* render pipelines and swap between them with ease
	*/
	class RenderPipeline {
		
	private:
		RenderQueue* renderQueue;

	public:
		RenderPipeline(RenderQueue* renderQueue);
		virtual ~RenderPipeline(); // Deletes the renderQueue

		RenderQueue& getRenderQueue() const;

		/* Enqueues a render command to be executed in the next rendered frame, 
		* returns false if it fails to enqueue the command, when it is invalid (i.e null mesh or material)
		*/
		virtual bool enqueueRender(const RenderCommand& renderCommand);
		/* Enqueues a post processing command to be executed in the next rendered frame,
		* returns false if it fails to enqueue the command, when it is invalid (TODO )
		*/
		virtual bool enqueuePostProcessing(const PostProcessingCommand& postProcessingCommand);

		/* Render to the default frame buffer */
		virtual void render(const Camera& camera, const Lights& lights) = 0;

		/* Render to a custom framebuffer */
		virtual void renderToFramebuffer(const Camera& camera, const Lights& lights, const FrameBuffer& frameBuffer) = 0;
	};

}

#endif
