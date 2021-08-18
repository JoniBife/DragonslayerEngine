#ifndef DEFERRED_RENDER_PIPELINE_H
#define DEFERRED_RENDER_PIPELINE_H

#include "../renderer/RenderPipeline.h"
#include "OpenGLState.h"
#include "../meshes/Mesh.h"

namespace renderer {

	class DeferredRenderPipeline : public RenderPipeline {

	private:
		OpenGLState* openGLState;

		ShaderProgram* geometryShaderProgram;
		ShaderProgram* shadowMapShaderProgram;
		ShaderProgram* pbrShaderProgram;
		ShaderProgram* postProcessingShaderProgram;

		FrameBuffer* gBuffer;
		std::vector<FrameBuffer*> shadowMapBuffers;
		FrameBuffer* lightBuffer;
		FrameBuffer* prePostProcessingBuffer;
		FrameBuffer* postProcessingBuffer;

		Mesh* quadNDC;

		unsigned int maxShadowMaps = 3;

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
		void renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget) override;
	};

}


#endif
