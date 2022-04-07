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

#define DEBUG_RENDERER

namespace WarriorRenderer {

	enum class RenderPass {
		GEOMETRY,
		SSAO,
		SHADOW,
		LIGHT,
		SKYBOX,
		POSTPROCESSING
	};

	class Renderer {

	private:
		// Global 
		OpenGLState openGLState;
		RenderQueue renderQueue;
		RenderingConfigurations renderingConfigurations;
		GLuint uboGlobalUniforms;
		
		// Geometry
		ShaderProgram geometryShaderProgram;
		FrameBuffer gBuffer;

		// SSAO 
		ShaderProgram ssaoShaderProgram;
		ShaderProgram ssaoBlurShaderProgram;
		FrameBuffer ssaoBuffer;
		FrameBuffer ssaoBlurBuffer;
		std::vector<Vec3> samples;
		unsigned int sampleSize = 64u;
		unsigned int noiseSize = 4u;
		GLuint noiseTexture;

		// Shadows 
		ShaderProgram shadowMapShaderProgram;
		std::vector<FrameBuffer> shadowMapBuffers;

		// Blur 
		ShaderProgram horizontalBlurShaderProgram;
		ShaderProgram verticalBlurShaderProgram;
		FrameBuffer horizontalBlurBuffer;
		FrameBuffer verticalBlurBuffer;
		const unsigned int blurWidth = 426;
		const unsigned int blurHeight = 240;

		// PBR
		ShaderProgram pbrShaderProgram;
		FrameBuffer lightBuffer;
		CubeMap irradianceCubeMap;
		CubeMap prefilterCubeMap;
		Texture2D brdfLUT;
		Texture2D* defaultAlbedoMap;
		Texture2D* defaultNormalMap;
		Texture2D* defaultMetallicMap;
		Texture2D* defaultRoughnessMap;
		Texture2D* defaultAOMap;

		// Lighting
		GLuint uboPointLights;
		Mesh* pointLightVolume;
		const unsigned int maxPointLights = 1000;

		// Skybox
		ShaderProgram skyboxShaderProgram;
		FrameBuffer skyboxBuffer;
		Mesh* cube;
		CubeMap* skyBox;

		//Post processing
		FrameBuffer postProcessingBuffer;
		FrameBuffer postProcessingBuffer2;
		Mesh* quadNDC;

		float blend = 0.1f;

#ifdef DEBUG_RENDERER
		// Contains the frame time for each of the render passes
		std::unordered_map<RenderPass, double> renderPassesFrameTime;
#endif

		/* Creates empty global uniform buffer */
		void createGlobalUniformsBuffer();
		void updateGlobalUniformsBuffer(const Mat4& view, const Mat4& projection);

		void createPointLightsBuffer();
		void updatePointLightsBuffer(const std::vector<PointLight>& pointLights);
		void bindPointLightsBuffer(GLuint index);

		void doGeometryPass(const Camera& camera);
		void doSSAOPass(const Camera& camera);
		void doShadowPass(const Camera& camera, const Lights& lights, std::vector<Mat4>& lightViewProjections);
		void doLightingPass(const Camera& camera, const Lights& lights, const std::vector<Mat4>& lightViewProjections);
		void doSkyBoxPass();
		FrameBuffer& doPostProcessingPasses(const Camera& camera, PostProcessingCommand& postProcessingCommand, Texture2D& previousRenderTexture, unsigned int currPass = 0u);

	public:
		/* Performs all initialization operations:
		* - Creation of OpenGL context
		* - Setting all default OpenGL states (face culling etc..)
		* - Loading and compilation of shaders
		* - Creation of intermediate framebuffers for each of the passes 
		* - Loading necessary meshes (Cube and Quad NDC)
		* - Loading default material */
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

		/* Returns the frame time in milliseconds for a specific pass
		only works if the DEBUG_RENDERER directive is active */
		double getFrameTime(const RenderPass& renderPass) const;
		GLint getBufferTexture();

		/* Creates a material using the default textures and settings */
		Material* createMaterial() const;
	};

}


#endif
