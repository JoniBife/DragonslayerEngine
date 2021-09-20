#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGLState.h"
#include "RenderQueue.h"
#include "textures/CubeMap.h"
#include "textures/FrameBuffer.h"
#include "PointLight.h"
#include "Lights.h"
#include "RenderTarget.h"
#include "view/Camera.h"
#include "Material.h"
#include "RenderingConfigurations.h"

namespace WarriorRenderer {

	class Renderer {

	private:
		// Both initialized by the default constructor
		OpenGLState openGLState;
		RenderQueue renderQueue;

		ShaderProgram geometryShaderProgram;
		ShaderProgram shadowMapShaderProgram;

		// Two-pass Gaussian blur shaders
		ShaderProgram horizontalBlurShaderProgram;
		ShaderProgram verticalBlurShaderProgram;

		ShaderProgram pbrShaderProgram;

		ShaderProgram skyboxShaderProgram;

		FrameBuffer gBuffer;
		std::vector<FrameBuffer> shadowMapBuffers;
		FrameBuffer lightBuffer;
		FrameBuffer skyboxBuffer;
		FrameBuffer postProcessingBuffer;
		FrameBuffer postProcessingBuffer2;
		FrameBuffer horizontalBlurBuffer;
		FrameBuffer verticalBlurBuffer;

		Texture2D* defaultAlbedoMap;
		Texture2D* defaultNormalMap;
		Texture2D* defaultMetallicMap;
		Texture2D* defaultRoughnessMap;
		Texture2D* defaultAOMap;

		Mesh* quadNDC;
		Mesh* cube;
		Mesh* pointLightVolume;

		CubeMap* skyBox;
		
		CubeMap irradianceCubeMap;
		CubeMap prefilterCubeMap;
		Texture2D brdfLUT;

		RenderingConfigurations renderingConfigurations;
		const unsigned int maxPointLights = 1000;
		const unsigned int blurWidth = 426;
		const unsigned int blurHeight = 240;

		GLuint uboPointLights;
		GLuint uboGlobalUniforms;

		/* Creates empty global uniform buffer */
		void createGlobalUniformsBuffer();
		void updateGlobalUniformsBuffer(const Mat4& view, const Mat4& projection);

		void createPointLightsBuffer();
		void updatePointLightsBuffer(const std::vector<PointLight>& pointLights);
		void bindPointLightsBuffer(GLuint index);

		void doGeometryPass(const Camera& camera);
		void doShadowPass(const Camera& camera, const Lights& lights, std::vector<Mat4>& lightViewProjections);
		void doLightingPass(const Camera& camera, const Lights& lights, const std::vector<Mat4>& lightViewProjections);
		void doSkyBoxPass();
		FrameBuffer& doPostProcessingPasses(PostProcessingCommand& postProcessingCommand, Texture2D& previousRenderTexture, unsigned int currPass = 0u);

	public:
		/* Performs all initialization operations:
		* - Creation of OpenGL context
		* - Setting all default OpenGL states (face culling etc..)
		* - Loading and compilation of shaders
		* - Creation of intermediate framebuffers for each of the passes */
		Renderer(const RenderingConfigurations& renderingConfigurations);
		~Renderer();

		void render(const Camera& camera, const Lights& lights);
		void renderToTarget(const Camera& camera, const Lights& lights, RenderTarget& renderTarget);

		/* Enqueues a render command to be executed in the next rendered frame,
		* returns false if it fails to enqueue the command, when it is invalid (i.e null mesh or material)
		*/
		bool enqueueRender(RenderCommand* renderCommand);
		/* Enqueues a post processing command to be executed in the next rendered frame,
		* returns false if it fails to enqueue the command, when it is invalid (TODO )
		*/
		bool enqueuePostProcessing(PostProcessingCommand* postProcessingCommand);

		void updateConfigutations(const RenderingConfigurations& newRenderingConfigurations);

		/* Creates a material using the default textures and settings */
		Material* createMaterial() const;
	};

}


#endif
