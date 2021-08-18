#ifndef DEFERRED_RENDER_PIPELINE_H
#define DEFERRED_RENDER_PIPELINE_H

#include "../renderer/RenderPipeline.h"
#include "OpenGLState.h"

namespace renderer {

	class DeferredRenderPipeline : public RenderPipeline {

	private:
		OpenGLState* openGLState;

		DeferredRenderQueue* deferredRenderQueue;

		ShaderProgram* geometryShaderProgram;
		ShaderProgram* shadowMapShaderProgram;
		ShaderProgram* pbrShaderProgram;
		ShaderProgram* postProcessingShaderProgram;

		FrameBuffer* gBuffer;
		std::vector<FrameBuffer*> shadowMapBuffers;
		FrameBuffer* prePostProcessingBuffer;
		FrameBuffer* postProcessingBuffer;

		Mesh* quadNDC;

		unsigned int maxShadowMaps = 3;

		/* Performs all initialization operations:
		* - Creation of OpenGL context
		* - Setting all default OpenGL states (face culling etc..)
		* - Loading and compilation of shaders
		* - Creation of intermediate framebuffers for each of the passes
		*/
		DeferredRenderPipeline(DeferredRenderQueue* deferredRenderQueue);

		static DeferredRenderPipeline* create();

	public:
		~DeferredRenderPipeline() override;

		void render(const Camera& camera, const Lights& lights) override;
		void renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget) override;
	};

}


#endif
