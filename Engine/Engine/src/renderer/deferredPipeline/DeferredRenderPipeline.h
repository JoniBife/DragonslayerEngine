#ifndef DEFERRED_RENDER_PIPELINE_H
#define DEFERRED_RENDER_PIPELINE_H

#include "../renderer/RenderPipeline.h"
#include "OpenGLState.h"

namespace renderer {

	class DeferredRenderPipeline : public RenderPipeline {

	private:
		OpenGLState* openGLState;

		Shader* geometryShader;
		Shader* shadowMapShader;

		FrameBuffer* gBuffer;
		std::vector<FrameBuffer*> shadowMapBuffers;
		FrameBuffer* postProcessingBuffer;

	public:
		/* Performs all initialization operations:
		* - Creation of OpenGL context
		* - Setting all default OpenGL states (face culling etc..)
		* - Loading and compilation of shaders
		* - Creation of intermediate framebuffers for each of the passes
		*/
		DeferredRenderPipeline();
		~DeferredRenderPipeline() override;

		void render(const Camera& camera, const Lights& lights) override;
		void renderToFramebuffer(const Camera& camera, const Lights& lights, const FrameBuffer& frameBuffer) override;
	};

}


#endif
