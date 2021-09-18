#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGLState.h"
#include "RenderQueue.h"
#include "../textures/CubeMap.h"
#include "../textures/FrameBuffer.h"
#include "PointLight.h"
#include "Lights.h"
#include "RenderTarget.h"
#include "../view/Camera.h"
#include "PMaterial.h"

namespace WarriorRenderer {

	class Renderer {

	private:
		OpenGLState* openGLState;

		RenderQueue renderQueue;

		ShaderProgram* geometryShaderProgram;
		ShaderProgram* shadowMapShaderProgram;

		// Two-pass Gaussian blur shaders
		ShaderProgram* horizontalBlurShaderProgram;
		ShaderProgram* verticalBlurShaderProgram;

		ShaderProgram* pbrShaderProgram;
		ShaderProgram* postProcessingShaderProgram;

		ShaderProgram* fxaaShaderProgram;

		ShaderProgram* skyboxShaderProgram;

		FrameBuffer* gBuffer;
		std::vector<FrameBuffer*> shadowMapBuffers;
		FrameBuffer* lightBuffer;
		FrameBuffer* skyboxBuffer;
		FrameBuffer* prePostProcessingBuffer;
		FrameBuffer* postProcessingBuffer;
		FrameBuffer* horizontalBlurBuffer;
		FrameBuffer* verticalBlurBuffer;

		Texture2D* defaultAlbedoMap;
		Texture2D* defaultNormalMap;
		Texture2D* defaultMetallicMap;
		Texture2D* defaultRoughnessMap;
		Texture2D* defaultAOMap;

		Mesh* quadNDC;
		Mesh* cube;
		Mesh* pointLightVolume;

		CubeMap* skyBox;
		CubeMap* irradianceCubeMap;
		CubeMap* prefilterCubeMap;
		Texture2D* brdfLUT;
		
		const unsigned int maxPointLights = 1000;
		const unsigned int maxShadowMaps = 3;
		const unsigned int blurWidth = 426;
		const unsigned int blurHeight = 240;

		unsigned int renderWidth = 1366;
		unsigned int renderHeight = 768;

		GLuint uboPointLights;
		GLuint uboGlobalUniforms;

		/* Creates empty global uniform buffer */
		void createGlobalUniformsBuffer();
		void updateGlobalUniformsBuffer(const Mat4& view, const Mat4& projection);

		void createPointLightsBuffer();
		void updatePointLightsBuffer(const std::vector<PointLight>& pointLights);
		void bindPointLightsBuffer(GLuint index);

	public:
		/* Performs all initialization operations:
		* - Creation of OpenGL context
		* - Setting all default OpenGL states (face culling etc..)
		* - Loading and compilation of shaders
		* - Creation of intermediate framebuffers for each of the passes */
		Renderer();
		~Renderer();

		void render(const Camera& camera, const Lights& lights);
		void renderToTarget(const Camera& camera, const Lights& lights, RenderTarget& renderTarget);

		/* Enqueues a render command to be executed in the next rendered frame,
		* returns false if it fails to enqueue the command, when it is invalid (i.e null mesh or material)
		*/
		bool enqueueRender(const RenderCommand& renderCommand);
		/* Enqueues a post processing command to be executed in the next rendered frame,
		* returns false if it fails to enqueue the command, when it is invalid (TODO )
		*/
		bool enqueuePostProcessing(const PostProcessingCommand& postProcessingCommand);

		PMaterial* createMaterial();
	};

}


#endif
