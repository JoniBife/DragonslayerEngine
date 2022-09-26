#include <glad/glad.h>
#include "DragonslayerEngine/Renderer.h"
#include "DragonslayerEngine/utils/OpenGLUtils.h"
#include "DragonslayerEngine/Material.h"
#include "view/Transformations.h"
#include <LMath/MathAux.hpp>
#include "textures/stb_image_write.h"
#include <string>
#include "DragonslayerEngine/textures/FrameBuffer.h"
#include "DragonslayerEngine/Mesh.h"
#include "DragonslayerEngine/textures/CubeMap.h"
#include <chrono>
#include <random>

using namespace LMath;

using namespace DragonslayerEngine;

static void printOpenGLInfo()
{
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
    std::cout << "OpenGL version " << version << std::endl;
    std::cout << "GLSL version " << glslVersion << std::endl;
}

void DragonslayerEngine::Renderer::createGlobalUniformsBuffer()
{
	GL_CALL(glGenBuffers(1, &uboGlobalUniforms));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uboGlobalUniforms));
	{
		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 2, 0, GL_DYNAMIC_DRAW));
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobalUniforms));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void DragonslayerEngine::Renderer::updateGlobalUniformsBuffer(const Mat4& view, const Mat4& projection)
{
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uboGlobalUniforms));
	{
		float viewOpenGLFormat[16];
		float projectionOpenGLFormat[16];
		view.toOpenGLFormat(viewOpenGLFormat);
		projection.toOpenGLFormat(projectionOpenGLFormat);
		GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(viewOpenGLFormat), viewOpenGLFormat));
		GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(viewOpenGLFormat), sizeof(projectionOpenGLFormat), projectionOpenGLFormat));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void DragonslayerEngine::Renderer::createPointLightsBuffer()
{
	GL_CALL(glGenBuffers(1, &uboPointLights));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uboPointLights));
	{
		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * maxPointLights, 0, GL_DYNAMIC_DRAW));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void DragonslayerEngine::Renderer::updatePointLightsBuffer(const std::vector<PointLight>& pointLights)
{
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uboPointLights));
	{
		if (pointLights.size() > 0) {
			GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight) * pointLights.size(), &pointLights[0]));
		} else {
			GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight) * maxPointLights, 0));
		}
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void DragonslayerEngine::Renderer::bindPointLightsBuffer(GLuint index)
{
	GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, uboPointLights));
}

void DragonslayerEngine::Renderer::doGeometryPass(const Camera& camera)
{
	gBuffer.bind();

	gBuffer.drawBuffers();
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	geometryShaderProgram.use();
	geometryShaderProgram.bindUniformBlock(geometryShaderProgram.getUniformBlockIndex("sharedMatrices"), 0);

	Mat4 normal;
	Material* material;
	Mesh* mesh;

	while (!renderQueue.isGeometryEmpty()) {

		RenderCommand& renderCommand = renderQueue.dequeueGeometry();
		material = renderCommand.material;
		mesh = renderCommand.mesh;

		material->getAlbedoMap().bind(GL_TEXTURE0);
		material->getNormalMap().bind(GL_TEXTURE1);
		material->getMetallicMap().bind(GL_TEXTURE2);
		material->getRoughnessMap().bind(GL_TEXTURE3);
		material->getAOMap().bind(GL_TEXTURE4);

		renderCommand.model.inverse(normal);

		geometryShaderProgram.setUniform("modelMatrix", renderCommand.model);
		geometryShaderProgram.setUniform("normalMatrix", normal.transpose().toMat3());
		geometryShaderProgram.setUniform("albedoTint", material->getAlbedoTint());
		geometryShaderProgram.setUniform("normalStrength", material->getNormalStrength());
		geometryShaderProgram.setUniform("metallicFactor", material->getMetallic());
		geometryShaderProgram.setUniform("roughnessFactor", material->getRoughness());
		geometryShaderProgram.setUniform("aoFactor", material->getAO());

		geometryShaderProgram.setUniform("albedoMap", 0);
		geometryShaderProgram.setUniform("normalMap", 1);
		geometryShaderProgram.setUniform("metallicMap", 2);
		geometryShaderProgram.setUniform("roughnessMap", 3);
		geometryShaderProgram.setUniform("ambientOcclusionMap", 4);

		mesh->bind();
		mesh->draw();
		mesh->unBind();
	}

	geometryShaderProgram.stopUsing();

	gBuffer.unbind();
}

void DragonslayerEngine::Renderer::doSSAOPass(const Camera& camera)
{
	ssaoBuffer.bind();
	ssaoShaderProgram.use();

	GL_CALL(glViewport(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight));

	ssaoShaderProgram.setUniform("gBufferNormalRoughness", 0);
	gBuffer.getColorAttachment(1).bind(GL_TEXTURE0);

	ssaoShaderProgram.setUniform("gBufferDepth", 1);
	gBuffer.getDepthAttachment().bind(GL_TEXTURE1);

	ssaoShaderProgram.setUniform("noise", 2);
	GL_CALL(glActiveTexture(GL_TEXTURE2));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, noiseTexture));

	for (unsigned int i = 0u; i < sampleSize; ++i) {
		std::string uniformName = "samples[" + std::to_string(i) + "]";
		ssaoShaderProgram.setUniform(uniformName, samples[i]);
	}
	ssaoShaderProgram.setUniform("sampleSize", sampleSize);

	ssaoShaderProgram.setUniform("projectionMatrix", camera.getProjection());
	//shaderProgram.setUniform("viewMatrix", camera.getView());
	Mat4 inverseProjection;
	camera.getProjection().inverse(inverseProjection);
	Mat4 inverseView;
	camera.getView().inverse(inverseView);
	ssaoShaderProgram.setUniform("inverseProjectionMatrix", inverseProjection);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();
	
	ssaoShaderProgram.stopUsing();
	ssaoBuffer.unbind();

	ssaoBlurBuffer.bind();
	ssaoBlurShaderProgram.use();

	GL_CALL(glViewport(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight));

	ssaoBlurShaderProgram.setUniform("dottedSSAO", 0);
	ssaoBuffer.getColorAttachment(0).bind(GL_TEXTURE0);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();

	ssaoBlurShaderProgram.stopUsing();
	ssaoBlurBuffer.unbind();
}

void DragonslayerEngine::Renderer::doShadowPass(const Camera& camera, const Lights& lights, std::vector<Mat4>& lightViewProjections)
{
	std::deque<RenderCommand*>& shadowMapCommands = renderQueue.getShadowMapQueue();

	if (shadowMapCommands.size() > 0 && !lights.directionalLights.empty()) {

		// TODO this should not be hardcoded
		//openGLState.setCullFace(GL_FRONT);
		//openGLState.setFaceCulling(false);

		// TODO calculate shadows from all directional lights

		for (int j = 0; j < shadowMapBuffers.size(); ++j) {

			shadowMapBuffers[j].bind();

			openGLState.setViewPort(0, 0, shadowMapBuffers[j].getWidth(), shadowMapBuffers[j].getHeigth());

			GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

			shadowMapShaderProgram.use();

			Mat4 lightView = lookAt(-1.0f * lights.directionalLights[0].direction, Vec3::ZERO, Vec3::Y);

			Mat4 inverseCameraView;
			camera.getView().inverse(inverseCameraView);

			// First cascade uses the near camera plane 
			float nearPlane = j == 0 ? camera.getNearPlane() : renderingConfigurations.cascadesPlanes[j - 1];
			float farPlane = renderingConfigurations.cascadesPlanes[j];

			Mat4 cascadeProjection = orthoCascade(
				nearPlane,
				farPlane,
				degreesToRadians(camera.getFov()),
				camera.getAspectRatio(),
				inverseCameraView,
				lightView);

			lightViewProjections.push_back(cascadeProjection * lightView);

			shadowMapShaderProgram.setUniform("lightSpaceProjectionMatrix", lightViewProjections[j]);

			for (RenderCommand* shadowMapCommand : shadowMapCommands) {

				shadowMapShaderProgram.setUniform("modelMatrix", shadowMapCommand->model);
				shadowMapCommand->mesh->bind();
				shadowMapCommand->mesh->draw();
				shadowMapCommand->mesh->unBind();
			}

			shadowMapShaderProgram.stopUsing();
			shadowMapBuffers[j].unbind();

			//ImGui::Image((ImTextureID)shadowMapBuffers[j].getColorAttachment(0).getId(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
		}

		renderQueue.clearShadowMapQueue();
		//openGLState.setCullFace(GL_BACK);
		//openGLState.setFaceCulling(true);

	}
}

void DragonslayerEngine::Renderer::doLightingPass(const Camera& camera, const Lights& lights, const std::vector<Mat4>& lightViewProjections)
{
	lightBuffer.bind();

	GL_CALL(glViewport(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight));

	lightBuffer.drawBuffers();

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); // Clearing all buffer attachments, MUST be done after drawBuffers
	pbrShaderProgram.use();

	pbrShaderProgram.setUniform("viewMatrix", camera.getView());
	Mat4 inverseProjection;
	camera.getProjection().inverse(inverseProjection);
	Mat4 inverseView;
	camera.getView().inverse(inverseView);
	pbrShaderProgram.setUniform("inverseViewMatrix", inverseView);
	pbrShaderProgram.setUniform("inverseProjectionMatrix", inverseProjection);

	// Sending directional lights
	DirectionalLight directionalLight;
	for (unsigned int i = 0; i < lights.directionalLights.size(); ++i) {
		directionalLight = lights.directionalLights[i];
		pbrShaderProgram.setUniform("directionalLights[" + std::to_string(i) + "].direction", directionalLight.direction.normalize());
		pbrShaderProgram.setUniform("directionalLights[" + std::to_string(i) + "].color", directionalLight.color);
		pbrShaderProgram.setUniform("directionalLights[" + std::to_string(i) + "].radiance", directionalLight.intensity);
	}
	pbrShaderProgram.setUniform("numberOfDirectionalLights", (unsigned int)lights.directionalLights.size());

	// Sending point lights
	updatePointLightsBuffer(lights.pointLights);
	GLuint index = pbrShaderProgram.getUniformBlockIndex("pointLightsBlock");
	bindPointLightsBuffer(1);
	pbrShaderProgram.bindUniformBlock(index, 1);
	pbrShaderProgram.setUniform("numberOfPointLights", (unsigned int)lights.pointLights.size());

	pbrShaderProgram.setUniform("viewPosition", camera.getPosition());
	pbrShaderProgram.setUniform("gBufferMetallic", 0);
	pbrShaderProgram.setUniform("gBufferNormalRoughness", 1);
	pbrShaderProgram.setUniform("gBufferAlbedoAmbientOcclusion", 2);
	pbrShaderProgram.setUniform("gBufferDepth", 3);
	pbrShaderProgram.setUniform("ssao", 4);
	pbrShaderProgram.setUniform("irradianceCubeMap", 5);
	pbrShaderProgram.setUniform("prefilterCubeMap", 6);
	pbrShaderProgram.setUniform("brdfLUT", 7);

	gBuffer.getColorAttachment(0).bind(GL_TEXTURE0);
	gBuffer.getColorAttachment(1).bind(GL_TEXTURE1);
	gBuffer.getColorAttachment(2).bind(GL_TEXTURE2);
	gBuffer.getDepthAttachment().bind(GL_TEXTURE3);

	// SSAO Texture
	ssaoBlurBuffer.getColorAttachment(0).bind(GL_TEXTURE4);

	irradianceCubeMap.bind(GL_TEXTURE5);
	prefilterCubeMap.bind(GL_TEXTURE6);
	brdfLUT.bind(GL_TEXTURE7);

	// Sending shadow parameters
	if (!lights.directionalLights.empty()) {
		pbrShaderProgram.setUniform("cascadesPlanes[0]", camera.getNearPlane());
		for (int i = 0; i < renderingConfigurations.cascadesPlanes.size(); ++i) {
			pbrShaderProgram.setUniform("cascadesPlanes[" + std::to_string(i + 1) + "]",
				renderingConfigurations.cascadesPlanes[i]);
			pbrShaderProgram.setUniform("shadowMaps[" + std::to_string(i) + "]", 8 + i);
			shadowMapBuffers[i].getDepthAttachment().bind(GL_TEXTURE8 + i);
			pbrShaderProgram.setUniform("lightViewProjectionMatrices[" + std::to_string(i) + "]", lightViewProjections[i]);
		}
		pbrShaderProgram.setUniform("numberOfCascades", (unsigned int)renderingConfigurations.cascadesPlanes.size());
	}

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();

	pbrShaderProgram.stopUsing();

	lightBuffer.unbind();
}

void DragonslayerEngine::Renderer::doSkyBoxPass() {

	// Copy depth buffer from gBuffer to skyboxBuffer and copy color buffer from light buffer to skyboxBuffer
	FrameBuffer::copyPixels(gBuffer, skyboxBuffer, GL_DEPTH_BUFFER_BIT);
	FrameBuffer::copyPixels(lightBuffer, skyboxBuffer);

	skyboxBuffer.bind();
	skyboxShaderProgram.use();
	skyboxShaderProgram.setUniform("skybox", 0);
	skyBox->bind(GL_TEXTURE0);

	openGLState.setFaceCulling(false);
	openGLState.setDepthFunction(GL_LEQUAL);

	cube->bind();
	cube->draw();
	cube->unBind();

	openGLState.setFaceCulling(true);
	openGLState.setDepthFunction(GL_LESS);

	skyBox->unBind(GL_TEXTURE0);
	skyboxShaderProgram.stopUsing();
	skyboxBuffer.unbind();
}

FrameBuffer& DragonslayerEngine::Renderer::doPostProcessingPasses(const Camera& camera, PostProcessingCommand& postProcessingCommand, Texture2D& previousRenderTexture,
	unsigned int currPass) {

	// Alternate between buffers inbetween post processing passes
	if (currPass % 2u == 0u) {
		postProcessingBuffer.bind();

		GL_CALL(glViewport(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight));

		auto shaderProgram = postProcessingCommand.getShader();
		shaderProgram.use();

		postProcessingCommand.sendParametersToShader(camera, gBuffer, previousRenderTexture);

		quadNDC->bind();
		quadNDC->draw();
		quadNDC->unBind();

		shaderProgram.stopUsing();
		postProcessingBuffer.unbind();

		++currPass;

		if (renderQueue.isPostProcessingEmpty())
			return postProcessingBuffer;

		return doPostProcessingPasses(camera, renderQueue.dequeuePostProcessing(), postProcessingBuffer.getColorAttachment(0), currPass);

	}
	else {
		postProcessingBuffer2.bind();

		GL_CALL(glViewport(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight));

		auto shaderProgram = postProcessingCommand.getShader();
		shaderProgram.use();

		postProcessingCommand.sendParametersToShader(camera, gBuffer, previousRenderTexture);

		quadNDC->bind();
		quadNDC->draw();
		quadNDC->unBind();

		shaderProgram.stopUsing();
		postProcessingBuffer2.unbind();

		++currPass;

		if (renderQueue.isPostProcessingEmpty())
			return postProcessingBuffer2;

		return doPostProcessingPasses(camera, renderQueue.dequeuePostProcessing(), postProcessingBuffer2.getColorAttachment(0), currPass);
	}
}

DragonslayerEngine::Renderer::Renderer(const RenderingConfigurations& renderingConfigurations, GLLoadProc load) {

	assert(renderingConfigurations.areValid());
	this->renderingConfigurations = renderingConfigurations;

	// 1. Creation of OpenGL context and loading all OpenGL functions 
	bool ableToLoadOpenGL = load ? gladLoadGLLoader(load) : gladLoadGL();
	if (!ableToLoadOpenGL)
	{
		std::cerr << "ERROR failed to load OpenGL" << std::endl;
		exit(EXIT_FAILURE);
	}

	// 2. Setting the renderer default OpenGL configurations 
#if _DEBUG
	//printOpenGLInfo();
#endif
	openGLState.setToDefaultState();

	std::vector<GLObject*> objectsToDelete;

	// 3. Loading and compilation of shaders
	Shader geometryVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/geometryVS.glsl");
	Shader geometryFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/geometryFS.glsl");
	geometryShaderProgram = ShaderProgram(geometryVS, geometryFS);
	objectsToDelete.push_back(&geometryVS);
	objectsToDelete.push_back(&geometryFS);

	Shader postProcessingVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/postProcessingVS.glsl");
	Shader ssaoFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/ssaoFS.glsl");
	ssaoShaderProgram = ShaderProgram(postProcessingVS, ssaoFS);
	objectsToDelete.push_back(&postProcessingVS);
	objectsToDelete.push_back(&ssaoFS);

	Shader ssaoBlurFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/ssaoBlurFS.glsl");
	ssaoBlurShaderProgram = ShaderProgram(postProcessingVS, ssaoBlurFS);
	objectsToDelete.push_back(&ssaoBlurFS);

	Shader depthVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/depthVS.glsl");
	Shader depthFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/depthFS.glsl");
	shadowMapShaderProgram = ShaderProgram(depthVS, depthFS);
	objectsToDelete.push_back(&depthVS);
	objectsToDelete.push_back(&depthFS);

	Shader pbrVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/pbrVS.glsl");
	Shader pbrFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/pbrFS.glsl");
	pbrShaderProgram = ShaderProgram(pbrVS, pbrFS);
	objectsToDelete.push_back(&pbrVS);
	objectsToDelete.push_back(&pbrFS);

	Shader gaussianBlurVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/deferred/gaussianBlurVS.glsl");
	Shader horizontalGaussianBlurFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/horizontalGaussianBlurFS.glsl");
	horizontalBlurShaderProgram = ShaderProgram(gaussianBlurVS, horizontalGaussianBlurFS);
	objectsToDelete.push_back(&gaussianBlurVS);
	objectsToDelete.push_back(&horizontalGaussianBlurFS);

	Shader verticalGaussianBlurFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/deferred/verticalGaussianBlurFS.glsl");
	verticalBlurShaderProgram = ShaderProgram(gaussianBlurVS, verticalGaussianBlurFS);
	objectsToDelete.push_back(&verticalGaussianBlurFS);

	Shader skyboxVS(GL_VERTEX_SHADER, "../DragonslayerEngine/shaders/skybox/skyboxVS.glsl");
	Shader skyboxFS(GL_FRAGMENT_SHADER, "../DragonslayerEngine/shaders/skybox/skyboxFS.glsl");
	skyboxShaderProgram = ShaderProgram(skyboxVS, skyboxFS);
	objectsToDelete.push_back(&skyboxVS);
	objectsToDelete.push_back(&skyboxFS);

	// 4. Creating intermediate framebuffers and global unifom buffer
	FrameBufferBuilder frameBufferBuilder;

	gBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_FLOAT, GL_RED) 
		.attachColorBuffers(2, GL_FLOAT, GL_RGBA)
		.attachDepthBuffer()
		.build();

	ssaoBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_FLOAT, GL_RED)
		.build();

	ssaoBlurBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_FLOAT, GL_RED)
		.build();

	for (int i = 0; i < renderingConfigurations.cascadesPlanes.size(); ++i) {
		// TODO Lower resolution for further cascades
		shadowMapBuffers.push_back(frameBufferBuilder
			.setSize(renderingConfigurations.shadowsResolution, renderingConfigurations.shadowsResolution)
			.attachColorBuffers(1, GL_HALF_FLOAT)
			.attachDepthBuffer()
			.build());
	}

	lightBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT) // Attaching color buffers with GL_RGBA16F precision to avoid clamping to [0,1]
		.build();

	skyboxBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.build();

	postProcessingBuffer = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();

	postProcessingBuffer2 = frameBufferBuilder
		.setSize(renderingConfigurations.renderWidth, renderingConfigurations.renderHeight)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();

	horizontalBlurBuffer = frameBufferBuilder
		.setSize(blurWidth, blurHeight)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();

	verticalBlurBuffer = frameBufferBuilder
		.setSize(blurWidth, blurHeight)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();

	createGlobalUniformsBuffer();

	createPointLightsBuffer();

	// 5. Creating the quad whose vertices will be used in most render passes and cube for the skybox
	quadNDC = Mesh::rectangle(2.0f, 2.0f);
	quadNDC->init();

	cube = Mesh::loadFromFile("../DragonslayerEngine/assets/objs/cube.obj");
	cube->init();

	pointLightVolume = Mesh::loadFromFile("../DragonslayerEngine/assets/objs/pointLightVolume.obj");
	pointLightVolume->init();

	// 6. Load default material textures
	defaultAlbedoMap = new Texture2D("../DragonslayerEngine/assets/textures/pbr/default/albedo.png");
	defaultNormalMap = new Texture2D("../DragonslayerEngine/assets/textures/pbr/default/normal.png");
	defaultMetallicMap = new Texture2D("../DragonslayerEngine/assets/textures/pbr/default/metallic.png");
	defaultRoughnessMap = new Texture2D("../DragonslayerEngine/assets/textures/pbr/default/roughness.png");
	defaultAOMap = new Texture2D("../DragonslayerEngine/assets/textures/pbr/default/ao.png");

	// 7. Load skybox
	std::vector<std::string> skyboxFaces = {
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap0.fa",
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap1.fa",
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap2.fa",
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap3.fa",
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap4.fa",
		"../DragonslayerEngine/assets/textures/irradiance/cubeMap5.fa"
	};
	skyBox = new CubeMap(CubeMap::fromFloatArrayFiles(skyboxFaces, 512, 512));

	// 8. Load irradiance map for indirect envirnment contribution
	std::vector<std::string> irradianceMapFaces = {
		"../DragonslayerEngine/assets/textures/irradiance/face0.fa",
		"../DragonslayerEngine/assets/textures/irradiance/face1.fa",
		"../DragonslayerEngine/assets/textures/irradiance/face2.fa",
		"../DragonslayerEngine/assets/textures/irradiance/face3.fa",
		"../DragonslayerEngine/assets/textures/irradiance/face4.fa",
		"../DragonslayerEngine/assets/textures/irradiance/face5.fa"
	};
	irradianceCubeMap = CubeMap::fromFloatArrayFiles(irradianceMapFaces, 32, 32);

	// 9. Load prefilter cube map
	std::vector<std::string> prefilterMapFacesMip0 = {
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face0.fa",
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face1.fa",
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face2.fa",
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face3.fa",
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face4.fa",
		"../DragonslayerEngine/assets/textures/irradiance/prefilterMip0face5.fa"
	};
	prefilterCubeMap = CubeMap::fromFloatArrayFiles(prefilterMapFacesMip0, 128, 128, true);
	std::string basePath = "../DragonslayerEngine/assets/textures/irradiance/prefilterMip";
	for (unsigned int mip = 1; mip < 5; ++mip) {

		std::string mipPath = basePath + std::to_string(mip);

		std::vector<std::string> mipFacesPaths;
		for (unsigned int face = 0; face < 6; ++face) {

			mipFacesPaths.push_back(mipPath + "face" + std::to_string(face) + ".fa");

		}
		prefilterCubeMap.addMipFromFloatArray(mipFacesPaths, mip);
	}

	// 10. Load brdf LUT texture
	brdfLUT = Texture2D::fromFloatArrayFile("../DragonslayerEngine/assets/textures/irradiance/brdf.fa", 512, 512);

	// 11. Load SSAO parameters
	// Not using rand because it's result is biased when using the mod operator
	// https://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-does-it-work
	std::uniform_real_distribution<float> dist(0.0, 1.0); // random floats between [0.0, 1.0]
	std::default_random_engine generator;

	// Generating samples in Tangent space around the normal oriented hemisphere (i.e. the z sample component is within the [0,1] interval)
	for (unsigned int i = 0; i < sampleSize; ++i)
	{
		Vec3 sample(
			dist(generator) * 2.0 - 1.0,
			dist(generator) * 2.0 - 1.0,
			dist(generator)
		);
		sample = sample.normalize();
		sample *= dist(generator);

		float scale = float(i) / sampleSize;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		samples.push_back(sample);
	}

	std::vector<Vec2> noise;
	// Generating a noise texture
	for (unsigned int i = 0; i < noiseSize * noiseSize; ++i)
	{
		Vec2 randomVec(
			dist(generator) * 2.0 - 1.0,
			dist(generator) * 2.0 - 1.0
		);
		noise.push_back(randomVec);
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, noiseSize, noiseSize, 0, GL_RG, GL_FLOAT, &noise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // We are tiling the texture over the whole screen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 12. Insert all render passes map entries
#ifdef DEBUG_RENDERER
	renderPassesFrameTime.insert({ RenderPass::GEOMETRY, -1.0f });
	renderPassesFrameTime.insert({ RenderPass::SSAO, -1.0f });
	renderPassesFrameTime.insert({ RenderPass::SHADOW, -1.0f });
	renderPassesFrameTime.insert({ RenderPass::LIGHT, -1.0f });
	renderPassesFrameTime.insert({ RenderPass::SKYBOX, -1.0f });
	renderPassesFrameTime.insert({ RenderPass::POSTPROCESSING, -1.0f });
#endif

	// 13. Finally delete any open gl objects that are no longer necesary
	for (GLObject* object : objectsToDelete) {
		object->deleteObject();
	}

}

DragonslayerEngine::Renderer::~Renderer()
{	
	// TODO Free any missing heap allocations and delete open gl object to free vram
	// Consider using the delete list
	geometryShaderProgram.deleteObject();
	shadowMapShaderProgram.deleteObject();

	horizontalBlurShaderProgram.deleteObject();
	verticalBlurShaderProgram.deleteObject();

	pbrShaderProgram.deleteObject();

	skyboxShaderProgram.deleteObject();

	gBuffer.deleteObject();
	for (FrameBuffer& frameBuffer : shadowMapBuffers) {
		frameBuffer.deleteObject();
	}

	lightBuffer.deleteObject();
	skyboxBuffer.deleteObject();
	postProcessingBuffer.deleteObject();
	postProcessingBuffer2.deleteObject();
	horizontalBlurBuffer.deleteObject();
	verticalBlurBuffer.deleteObject();

	defaultAlbedoMap->deleteObject();
	delete defaultAlbedoMap;
	defaultNormalMap->deleteObject();
	delete defaultNormalMap;
	defaultMetallicMap->deleteObject();
	delete defaultMetallicMap;
	defaultRoughnessMap->deleteObject();
	delete defaultRoughnessMap;
	defaultAOMap->deleteObject();
	delete defaultAOMap;

	quadNDC->deleteObject();
	delete quadNDC;
	cube->deleteObject();
	delete cube;
	pointLightVolume->deleteObject();
	delete pointLightVolume;

	irradianceCubeMap.deleteObject();
	prefilterCubeMap.deleteObject();
	brdfLUT.deleteObject();
}

void DragonslayerEngine::Renderer::updateConfigurations(const RenderingConfigurations& newRenderingConfigurations)
{
	assert(newRenderingConfigurations.areValid());

	// If the rendering resolution changes we need to update the size of the frame buffers
	if (newRenderingConfigurations.renderWidth != renderingConfigurations.renderWidth
		|| newRenderingConfigurations.renderHeight != renderingConfigurations.renderHeight) {

		gBuffer.resize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight);
		lightBuffer.resize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight);
		skyboxBuffer.resize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight);
		postProcessingBuffer.resize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight);
		postProcessingBuffer2.resize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight);
	}

	int cascadePlanesDiff = newRenderingConfigurations.cascadesPlanes.size() - renderingConfigurations.cascadesPlanes.size();
	if (cascadePlanesDiff > 0) {
		FrameBufferBuilder frameBufferBuilder;
		for (unsigned int i = 0u; i < cascadePlanesDiff; ++i) {
			shadowMapBuffers.push_back(frameBufferBuilder
				.setSize(newRenderingConfigurations.renderWidth, newRenderingConfigurations.renderHeight)
				.attachColorBuffers(1, GL_HALF_FLOAT)
				.attachDepthBuffer()
				.build());
		}

	} else if (cascadePlanesDiff < 0) {
		for (auto it = shadowMapBuffers.end() + cascadePlanesDiff; it != shadowMapBuffers.end(); ++it) {
			(*it).deleteObject();
		}
		shadowMapBuffers.erase(shadowMapBuffers.end() + cascadePlanesDiff, shadowMapBuffers.end());
	}

	// If the shadow resolution changes we also need to update the shadow map buffers
	if (newRenderingConfigurations.shadowsResolution != renderingConfigurations.shadowsResolution) {

		for (FrameBuffer& shadowMapBuffer : shadowMapBuffers) {
			shadowMapBuffer.resize(newRenderingConfigurations.shadowsResolution, newRenderingConfigurations.shadowsResolution);
		}
	}

	renderingConfigurations = newRenderingConfigurations;
}

void DragonslayerEngine::Renderer::render(const Camera& camera, const Lights& lights)
{
	GL_CALL(glClearColor(0, 0, 0, 1));

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	openGLState.setViewPort(0, 0, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight);
	openGLState.setDepthTesting(true);
	openGLState.setDepthFunction(GL_LESS);
	openGLState.setDepthRange(0.0, 1.0);
	openGLState.setFaceCulling(true);
	openGLState.setCullFace(GL_BACK);
	openGLState.setFrontFace(GL_CCW);
	openGLState.setBlending(false);
	GL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
	
	// TODO Consider caching to avoid updating the buffer everyframe
	updateGlobalUniformsBuffer(camera.getView(), camera.getProjection());

	// 1. Render all the geometry to the gBuffer
#ifdef DEBUG_RENDERER
	auto start = std::chrono::high_resolution_clock::now();
	doGeometryPass(camera);
	glFinish();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	renderPassesFrameTime[RenderPass::GEOMETRY] = duration.count();
#else 
	doGeometryPass(camera);
#endif

	// 2. Render ambient occlusion to post processing buffer
#ifdef DEBUG_RENDERER
	start = std::chrono::high_resolution_clock::now();
	doSSAOPass(camera);
	glFinish();
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	renderPassesFrameTime[RenderPass::SSAO] = duration.count();
#else 
	doSSAOPass(camera);
#endif

	// 3. Render from each of the lights perspective to generate each of the shadow maps
	std::vector<Mat4> lightViewProjections;

#ifdef DEBUG_RENDERER
	start = std::chrono::high_resolution_clock::now();
	doShadowPass(camera, lights, lightViewProjections);
	glFinish();
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	renderPassesFrameTime[RenderPass::SHADOW] = duration.count();
#else 
	doShadowPass(camera, lights, lightViewProjections);
#endif

	// 4. Render with lighting
#ifdef DEBUG_RENDERER
	start = std::chrono::high_resolution_clock::now();
	doLightingPass(camera, lights, lightViewProjections);
	glFinish();
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	renderPassesFrameTime[RenderPass::LIGHT] = duration.count();
#else
	doLightingPass(camera, lights, lightViewProjections);
#endif

	// 5. Render skybox
#ifdef DEBUG_RENDERER
	start = std::chrono::high_resolution_clock::now();
	doSkyBoxPass();
	glFinish();
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	renderPassesFrameTime[RenderPass::SKYBOX] = duration.count();
#else
	doSkyBoxPass();
#endif

	// 6. Apply post processing if there is any
#ifdef DEBUG_RENDERER
	start = std::chrono::high_resolution_clock::now();
	if (!renderQueue.isPostProcessingEmpty()) {
		FrameBuffer& finalFrameBuffer = doPostProcessingPasses(camera, renderQueue.dequeuePostProcessing(), skyboxBuffer.getColorAttachment(0));

		FrameBuffer::copyPixels(finalFrameBuffer, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight);
	} else {
		FrameBuffer::copyPixels(skyboxBuffer, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight);
	}
	glFinish();
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	renderPassesFrameTime[RenderPass::POSTPROCESSING] = duration.count();
#else
	if (!renderQueue.isPostProcessingEmpty()) {
		FrameBuffer& finalFrameBuffer = doPostProcessingPasses(camera,renderQueue.dequeuePostProcessing(), skyboxBuffer.getColorAttachment(0));

		FrameBuffer::copyPixels(finalFrameBuffer, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight);
}
	else {
		FrameBuffer::copyPixels(skyboxBuffer, renderingConfigurations.renderWidth, renderingConfigurations.renderHeight);
	}
#endif
}

void DragonslayerEngine::Renderer::renderToTarget(const Camera& camera, const Lights& lights, RenderTarget& renderTarget)
{
}

bool DragonslayerEngine::Renderer::enqueueRender(RenderCommand* renderCommand)
{
	return renderQueue.enqueueRender(renderCommand);
}

bool DragonslayerEngine::Renderer::enqueuePostProcessing(PostProcessingCommand* postProcessingCommand)
{
	return renderQueue.enqueuePostProcessing(postProcessingCommand);
}

double DragonslayerEngine::Renderer::getFrameTime(const RenderPass& renderPass) const
{
#ifdef DEBUG_RENDERER
	return renderPassesFrameTime.find(renderPass)->second;
#else
	return -1.0f;
#endif
}


GLint DragonslayerEngine::Renderer::getBufferTexture()
{
#ifdef DEBUG_RENDERER
	return ssaoBlurBuffer.getColorAttachment(0).getId();
#else
    return -1;
#endif
}


Material* DragonslayerEngine::Renderer::createMaterial() const
{
	return new Material(defaultAlbedoMap, defaultNormalMap, defaultMetallicMap, defaultRoughnessMap, defaultAOMap);
}


