#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include "../view/Camera.h"
#include "../textures/FrameBuffer.h"
#include "RenderQueue.h"
#include "Lights.h"
#include "RenderTarget.h"

namespace renderer {

	/*
	* Base class for any render pipeline (deferred, forward etc..)
	* This way the render is abstracted from this structure and can save multiple
	* render pipelines and swap between them with ease
	*/
	class RenderPipeline {

	protected:
		RenderQueue* renderQueue;

		unsigned int renderWidth = 1366;
		unsigned int renderHeight = 768;

	public:
		RenderPipeline(RenderQueue* renderQueue);
		virtual ~RenderPipeline(); // Deletes the renderQueue

		RenderQueue& getRenderQueue() const;

		void setRenderSize(unsigned int width, unsigned int height);
		Vec2 getRenderSize() const;

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

		/* Render to a custom render target (with a custom frame buffer attached) */
		virtual void renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget) = 0;
	};

}

#endif
