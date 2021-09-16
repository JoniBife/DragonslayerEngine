#include <GL/glew.h>
#include "DeferredRenderPipeline.h"
#include "DeferredRenderQueue.h"
#include "../../utils/OpenGLUtils.h"
#include "GLPBRMaterial.h"
#include "../view/Transformations.h"
#include <imgui/imgui.h>
#include "../../gui/ImGuiExtensions.h"
#include "../../math/MathAux.h"
#include "../../textures/std_image_write.h"

using namespace renderer;

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

void renderer::DeferredRenderPipeline::createGlobalUniformsBuffer()
{
	GL_CALL(glGenBuffers(1, &vboGlobalUniforms));
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, vboGlobalUniforms));
	{
		GL_CALL(glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 2, 0, GL_DYNAMIC_DRAW));
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, vboGlobalUniforms));
	}
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void renderer::DeferredRenderPipeline::updateGlobalUniformsBuffer(const Mat4& view, const Mat4& projection)
{
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, vboGlobalUniforms));
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

renderer::DeferredRenderPipeline::DeferredRenderPipeline() : RenderPipeline(new DeferredRenderQueue()) {

	// Casting once when the pipeline is created instead of everyframe
	deferredRenderQueue = dynamic_cast<DeferredRenderQueue*>(renderQueue);

	// 1. Creation of OpenGL context and loading all OpenGL functions 
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GL_CALL(GLenum result = glewInit());
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}

	// 2. Setting the renderer default OpenGL configurations 
#if _DEBUG
	printOpenGLInfo();
#endif
	openGLState = OpenGLState::createDefaultState();

	// 3. Loading and compilation of shaders
	Shader geometryVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/geometryVS.glsl");
	Shader geometryFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/geometryFS.glsl");
	geometryShaderProgram = new ShaderProgram(geometryVS, geometryFS);

	Shader depthVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/depthVS.glsl");
	Shader depthFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/depthFS.glsl");
	shadowMapShaderProgram = new ShaderProgram(depthVS, depthFS);

	Shader pbrVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/pbrVS.glsl");
	Shader pbrFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/pbrFS.glsl");
	pbrShaderProgram = new ShaderProgram(pbrVS, pbrFS);

	Shader postProcessingVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/postProcessingVS.glsl");
	Shader postProcessingFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/postProcessingFS.glsl");
	postProcessingShaderProgram = new ShaderProgram(postProcessingVS, postProcessingFS);

	Shader gaussianBlurVS(GL_VERTEX_SHADER, "../Engine/shaders/deferred/gaussianBlurVS.glsl");
	Shader horizontalGaussianBlurFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/horizontalGaussianBlurFS.glsl");
	horizontalBlurShaderProgram = new ShaderProgram(gaussianBlurVS, horizontalGaussianBlurFS);

	Shader verticalGaussianBlurFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/verticalGaussianBlurFS.glsl");
	verticalBlurShaderProgram = new ShaderProgram(gaussianBlurVS, verticalGaussianBlurFS);

	Shader fxaaFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/fxaaFS.glsl");
	fxaaShaderProgram = new ShaderProgram(postProcessingVS, fxaaFS);

	Shader skyboxVS(GL_VERTEX_SHADER, "../Engine/shaders/skybox/skyboxVS.glsl");
	Shader skyboxFS(GL_FRAGMENT_SHADER, "../Engine/shaders/skybox/skyboxFS.glsl");
	skyboxShaderProgram = new ShaderProgram(skyboxVS, skyboxFS);

	// 4. Creating intermediate framebuffers and global unifom buffer
	FrameBufferBuilder frameBufferBuilder;

	gBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(1, GL_FLOAT, GL_RED) 
		.attachColorBuffers(2, GL_FLOAT, GL_RGBA)
		.attachDepthBuffer()
		.build();

	/*for (int i = 0; i < maxShadowMaps; ++i) {
		// TODO ShadowMap depth texture has different properties
		shadowMapBuffers.push_back(frameBufferBuilder
			.setSize(1024, 1024)
			.attachColorBuffers(1, GL_HALF_FLOAT)
			.attachDepthBuffer()
			.build());
	}*/

	shadowMapBuffers.push_back(frameBufferBuilder
		.setSize(1024, 1024)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.build());

	shadowMapBuffers.push_back(frameBufferBuilder
		.setSize(1024, 1024)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.build());

	shadowMapBuffers.push_back(frameBufferBuilder
		.setSize(1024, 1024)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.build());

	lightBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT) // Attaching color buffers with GL_RGBA16F precision to avoid clamping to [0,1]
		.attachDepthBuffer()
		.attachStencilBuffer()
		.build();

	skyboxBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.build();

	prePostProcessingBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.attachStencilBuffer()
		.build();

	postProcessingBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
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

	// 5. Creating the quad whose vertices will be used in most render passes and cube for the skybox
	quadNDC = Mesh::rectangle(2.0f, 2.0f);
	quadNDC->init();

	cube = Mesh::loadFromFile("../Engine/objs/cube.obj");
	cube->init();

	// 6. Load default material textures
	defaultAlbedoMap = new Texture2D("../Engine/textures/pbr/default/albedo.png");
	defaultNormalMap = new Texture2D("../Engine/textures/pbr/default/normal.png");
	defaultMetallicMap = new Texture2D("../Engine/textures/pbr/default/metallic.png");
	defaultRoughnessMap = new Texture2D("../Engine/textures/pbr/default/roughness.png");
	defaultAOMap = new Texture2D("../Engine/textures/pbr/default/ao.png");

	// 7. Load skybox
	std::vector<std::string> skyboxFaces = {
		"../Engine/textures/irradiance/cubeMap0.fa",
		"../Engine/textures/irradiance/cubeMap1.fa",
		"../Engine/textures/irradiance/cubeMap2.fa",
		"../Engine/textures/irradiance/cubeMap3.fa",
		"../Engine/textures/irradiance/cubeMap4.fa",
		"../Engine/textures/irradiance/cubeMap5.fa"
	};
	skyBox = CubeMap::fromFloatArrayFiles(skyboxFaces, 512, 512);

	// 8. Load irradiance map for indirect envirnment contribution
	std::vector<std::string> irradianceMapFaces = {
		"../Engine/textures/irradiance/face0.fa",
		"../Engine/textures/irradiance/face1.fa",
		"../Engine/textures/irradiance/face2.fa",
		"../Engine/textures/irradiance/face3.fa",
		"../Engine/textures/irradiance/face4.fa",
		"../Engine/textures/irradiance/face5.fa"
	};
	irradianceCubeMap = CubeMap::fromFloatArrayFiles(irradianceMapFaces, 32, 32);

	// 9. Load prefilter cube map
	std::vector<std::string> prefilterMapFacesMip0 = {
		"../Engine/textures/irradiance/prefilterMip0face0.fa",
		"../Engine/textures/irradiance/prefilterMip0face1.fa",
		"../Engine/textures/irradiance/prefilterMip0face2.fa",
		"../Engine/textures/irradiance/prefilterMip0face3.fa",
		"../Engine/textures/irradiance/prefilterMip0face4.fa",
		"../Engine/textures/irradiance/prefilterMip0face5.fa"
	};
	prefilterCubeMap = CubeMap::fromFloatArrayFiles(prefilterMapFacesMip0, 128, 128, true);
	std::string basePath = "../Engine/textures/irradiance/prefilterMip";
	for (unsigned int mip = 1; mip < 5; ++mip) {

		std::string mipPath = basePath + std::to_string(mip);

		std::vector<std::string> mipFacesPaths;
		for (unsigned int face = 0; face < 6; ++face) {

			mipFacesPaths.push_back(mipPath + "face" + std::to_string(face) + ".fa");

		}
		prefilterCubeMap->addMipFromFloatArray(mipFacesPaths, mip);
	}

	// 10. Load brdf LUT texture
	brdfLUT = Texture2D::fromFloatArrayFile("../Engine/textures/irradiance/brdf.fa", 512, 512);
}

renderer::DeferredRenderPipeline::~DeferredRenderPipeline()
{
	delete openGLState;

	// Delete shader programs
	delete geometryShaderProgram;
	delete shadowMapShaderProgram;
	delete pbrShaderProgram;
	delete postProcessingShaderProgram;
	delete horizontalBlurShaderProgram;
	delete verticalBlurShaderProgram;
	
	// Delete buffers
	delete gBuffer;
	for (int i = 0; i < maxShadowMaps; ++i) {
		delete shadowMapBuffers[i];
	}

	delete prePostProcessingBuffer;
	delete horizontalBlurBuffer;
	delete verticalBlurBuffer;
}

void renderer::DeferredRenderPipeline::render(const Camera& camera, const Lights& lights)
{
	GL_CALL(glClearColor(0, 0, 0, 1));

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	openGLState->setViewPort(0, 0, renderWidth, renderHeight);
	openGLState->setDepthTesting(true);
	openGLState->setDepthFunction(GL_LESS);
	openGLState->setDepthRange(0.0, 1.0);
	openGLState->setFaceCulling(true);
	openGLState->setCullFace(GL_BACK);
	openGLState->setFrontFace(GL_CCW);
	openGLState->setBlending(false);
	
	// TODO Consider caching to avoid updating the buffer everyframe
	updateGlobalUniformsBuffer(camera.getView(), camera.getProjection());

	// 1. Render all the geometry to the gBuffer
	gBuffer->bind();
	
	gBuffer->drawBuffers();
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	geometryShaderProgram->use();
	geometryShaderProgram->bindUniformBlock(geometryShaderProgram->getUniformBlockIndex("sharedMatrices"), 0);

	RenderCommand renderCommand;
	Mat4 normal;
	GLPBRMaterial* material;
	Mesh* mesh;

	while (!deferredRenderQueue->isGeometryEmpty()) {

		renderCommand = deferredRenderQueue->dequeueGeometry();
		material = renderCommand.material;
		mesh = renderCommand.mesh;

		material->getAlbedoMap().bind(GL_TEXTURE0);
		material->getNormalMap().bind(GL_TEXTURE1);
		material->getMetallicMap().bind(GL_TEXTURE2);
		material->getRoughnessMap().bind(GL_TEXTURE3);
		material->getAOMap().bind(GL_TEXTURE4);

		renderCommand.model.inverse(normal);

		geometryShaderProgram->setUniform("modelMatrix", renderCommand.model);
		geometryShaderProgram->setUniform("normalMatrix", normal.transpose().toMat3());
		geometryShaderProgram->setUniform("albedoTint", material->getAlbedoTint());
		geometryShaderProgram->setUniform("normalStrength", material->getNormalStrength());
		geometryShaderProgram->setUniform("metallicFactor", material->getMetallic());
		geometryShaderProgram->setUniform("roughnessFactor", material->getRoughness());
		geometryShaderProgram->setUniform("aoFactor", material->getAO());

		geometryShaderProgram->setUniform("albedoMap", 0);
		geometryShaderProgram->setUniform("normalMap", 1);
		geometryShaderProgram->setUniform("metallicMap", 2);
		geometryShaderProgram->setUniform("roughnessMap", 3);
		geometryShaderProgram->setUniform("ambientOcclusionMap", 4);

		mesh->bind();
		mesh->draw();
		mesh->unBind();
	}

	geometryShaderProgram->stopUsing();
	
	gBuffer->unbind();

	// 2. Render from each of the lights perspective to generate each of the shadow maps
	auto shadowMapCommands = deferredRenderQueue->getShadowMapQueue();
	
	Mat4 lightViewProjections[3];

	static float cascades[] = { camera.getNearPlane(), 7.5f, 25.0f, 42.5f };
	static float far1 = cascades[1];
	static float far2 = cascades[2];
	ImGui::InputFloat("Far1", &far1);
	ImGui::InputFloat("Far2", &far2);

	if (ImGui::Button("Update planes")) {
		cascades[1] = far1;
		cascades[2] = far2;
	}

	static bool debugShadows = false;

	if (shadowMapCommands.size() > 0) {

		ImGui::Checkbox("Debug shadows", &debugShadows);
		
		// TODO this should not be hardcoded
		//openGLState->setCullFace(GL_FRONT);
		//openGLState->setFaceCulling(false);

		for (int i = 0; i < lights.directionalLights.size(); ++i) {

			for (int j = 0; j < shadowMapBuffers.size(); ++j) {
				
				shadowMapBuffers[j]->bind();

				openGLState->setViewPort(0, 0, shadowMapBuffers[j]->getWidth(), shadowMapBuffers[j]->getHeigth());

				GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

				shadowMapShaderProgram->use();

				Mat4 lightView = lookAt(-1.0f * lights.directionalLights[i].direction * 10.0f, Vec3::ZERO, Vec3::Y);

				Mat4 inverseCameraView;
				camera.getView().inverse(inverseCameraView);

				Mat4 cascadeProjection = orthoCascade(
					cascades[j],
					cascades[j+1],
					degreesToRadians(camera.getFov()),
					camera.getAspectRatio(),
					inverseCameraView,
					lightView);

				lightViewProjections[j] = cascadeProjection * lightView;

				shadowMapShaderProgram->setUniform("lightSpaceProjectionMatrix", lightViewProjections[j]);

				for (const RenderCommand shadowMapCommand : shadowMapCommands._Get_container()) {

					shadowMapShaderProgram->setUniform("modelMatrix", shadowMapCommand.model);
					shadowMapCommand.mesh->bind();
					shadowMapCommand.mesh->draw();
					shadowMapCommand.mesh->unBind();
				}

				shadowMapShaderProgram->stopUsing();
				shadowMapBuffers[j]->unbind();

				ImGui::Image((ImTextureID)shadowMapBuffers[j]->getColorAttachment(0).getId(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			}
		}

		// TODO Temporary solution to clean shadow queue
		while (!deferredRenderQueue->isShadowMapEmpty()) {
			deferredRenderQueue->dequeueShadowMap();
		}

		//openGLState->setCullFace(GL_BACK);
		//openGLState->setFaceCulling(true);
		
	}

	// 3. Render with lighting
	lightBuffer->bind();

	glViewport(0, 0, renderWidth, renderHeight);
	
	lightBuffer->drawBuffers();
	
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); // Clearing all buffer attachments, MUST be done after drawBuffers
	pbrShaderProgram->use();

	pbrShaderProgram->setUniform("viewMatrix", camera.getView());
	Mat4 inverseProjection;
	camera.getProjection().inverse(inverseProjection);
	Mat4 inverseView;
	camera.getView().inverse(inverseView);
	pbrShaderProgram->setUniform("inverseViewMatrix", inverseView);
	pbrShaderProgram->setUniform("inverseProjectionMatrix", inverseProjection);

	//pbrShaderProgram->setUniform("projectionNear", camera.getNearPlane());
	//pbrShaderProgram->setUniform("projectionFar", camera.getFarPlane());
	pbrShaderProgram->setUniform("far", cascades[1]);
	pbrShaderProgram->setUniform("far2", cascades[2]);

	pbrShaderProgram->setUniform("debug", debugShadows);

	pbrShaderProgram->setUniform("lightViewProjectionMatrix", lightViewProjections[0]);
	pbrShaderProgram->setUniform("lightViewProjectionMatrix2", lightViewProjections[1]);
	pbrShaderProgram->setUniform("lightViewProjectionMatrix3", lightViewProjections[2]);

	pbrShaderProgram->setUniform("viewPosition", camera.getPosition());
	pbrShaderProgram->setUniform("gBufferMetallic", 0);
	pbrShaderProgram->setUniform("gBufferNormalRoughness", 1);
	pbrShaderProgram->setUniform("gBufferAlbedoAmbientOcclusion", 2);
	pbrShaderProgram->setUniform("gBufferDepth", 3);
	pbrShaderProgram->setUniform("shadowMap", 4);
	pbrShaderProgram->setUniform("shadowMap2", 5);
	pbrShaderProgram->setUniform("shadowMap3", 6);
	pbrShaderProgram->setUniform("irradianceCubeMap", 7);
	pbrShaderProgram->setUniform("prefilterCubeMap", 8);
	pbrShaderProgram->setUniform("brdfLUT", 9);

	static Vec3 pixelOffset;
	ImGui::InputVec3("Pixel Offset", pixelOffset);
	pbrShaderProgram->setUniform("pixelOffset", pixelOffset);

	gBuffer->getColorAttachment(0).bind(GL_TEXTURE0);
	gBuffer->getColorAttachment(1).bind(GL_TEXTURE1);
	gBuffer->getColorAttachment(2).bind(GL_TEXTURE2);
	gBuffer->getDepthAttachment().bind(GL_TEXTURE3);
	shadowMapBuffers[0]->getDepthAttachment().bind(GL_TEXTURE4);
	shadowMapBuffers[1]->getDepthAttachment().bind(GL_TEXTURE5);
	shadowMapBuffers[2]->getDepthAttachment().bind(GL_TEXTURE6);
	irradianceCubeMap->bind(GL_TEXTURE7);
	prefilterCubeMap->bind(GL_TEXTURE8);
	brdfLUT->bind(GL_TEXTURE9);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();

	pbrShaderProgram->stopUsing();
	
	lightBuffer->unbind();


	// Render skybox
	// Copy depth buffer from gBuffer to skyboxBuffer and copy color buffer from light buffer to skyboxBuffer

	gBuffer->bind(GL_READ_FRAMEBUFFER);
	skyboxBuffer->bind(GL_DRAW_FRAMEBUFFER);

	GL_CALL(glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, renderWidth, renderHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
	lightBuffer->bind(GL_READ_FRAMEBUFFER);
	GL_CALL(glBlitFramebuffer(0, 0, renderWidth, renderHeight, 0, 0, renderWidth, renderHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST));

	skyboxBuffer->bind();
	skyboxShaderProgram->use();
	skyboxShaderProgram->setUniform("skybox", 0);
	skyBox->bind(GL_TEXTURE0);

	openGLState->setFaceCulling(false);
	openGLState->setDepthFunction(GL_LEQUAL);

	cube->bind();
	cube->draw();
	cube->unBind();

	openGLState->setFaceCulling(true);
	openGLState->setDepthFunction(GL_LESS);

	skyBox->unBind(GL_TEXTURE0);
	skyboxShaderProgram->stopUsing();
	skyboxBuffer->unbind();

	ImGui::Image((ImTextureID)skyboxBuffer->getColorAttachment(0).getId(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));

	//glViewport(0,0,renderWidth, renderHeight);

	// 4. Blur

	static int blurPasses = 0;
	ImGui::SliderInt("Blur passes", &blurPasses, 0, 10);
	if (blurPasses > 0) {

		glViewport(0, 0, blurWidth, blurHeight);

		horizontalBlurBuffer->bind();
		horizontalBlurBuffer->drawBuffers();

		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		horizontalBlurShaderProgram->use();
		horizontalBlurShaderProgram->setUniform("previousRenderTexture", 0);
		skyboxBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

		quadNDC->bind();
		quadNDC->draw();
		quadNDC->unBind();

		horizontalBlurShaderProgram->stopUsing();
		horizontalBlurBuffer->unbind();

		verticalBlurBuffer->bind();
		verticalBlurBuffer->drawBuffers();

		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		verticalBlurShaderProgram->use();
		verticalBlurShaderProgram->setUniform("previousRenderTexture", 0);
		horizontalBlurBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

		quadNDC->bind();
		quadNDC->draw();
		quadNDC->unBind();

		verticalBlurShaderProgram->stopUsing();

		verticalBlurBuffer->unbind();

		for (int i = 0; i < blurPasses - 1; ++i) {

			horizontalBlurBuffer->bind();
			horizontalBlurBuffer->drawBuffers();

			GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

			horizontalBlurShaderProgram->use();
			horizontalBlurShaderProgram->setUniform("previousRenderTexture", 0);
			verticalBlurBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

			quadNDC->bind();
			quadNDC->draw();
			quadNDC->unBind();

			horizontalBlurShaderProgram->stopUsing();
			horizontalBlurBuffer->unbind();

			verticalBlurBuffer->bind();
			verticalBlurBuffer->drawBuffers();

			GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

			verticalBlurShaderProgram->use();
			verticalBlurShaderProgram->setUniform("previousRenderTexture", 0);
			horizontalBlurBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

			quadNDC->bind();
			quadNDC->draw();
			quadNDC->unBind();

			verticalBlurShaderProgram->stopUsing();

			verticalBlurBuffer->unbind();

		}
	}

	// 3. Apply any post processing
	postProcessingBuffer->bind();
	postProcessingBuffer->drawBuffers();

	glViewport(0, 0, renderWidth, renderHeight);

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	postProcessingShaderProgram->use();
	postProcessingShaderProgram->setUniform("previousRenderTexture",0);

	static bool toneMapping = true;
	ImGui::Checkbox("ToneMapping", &toneMapping);

	postProcessingShaderProgram->setUniform("toneMapping", toneMapping);
	
	if (blurPasses == 0)
		skyboxBuffer->getColorAttachment(0).bind(GL_TEXTURE0);
	else
		verticalBlurBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();
	postProcessingShaderProgram->stopUsing();

	postProcessingBuffer->unbind();

	glViewport(0, 0, renderWidth, renderHeight);

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	fxaaShaderProgram->use();
	fxaaShaderProgram->setUniform("previousRenderTexture", 0);

	static bool fxaa = true;
	ImGui::Checkbox("FXAA", &fxaa);

	fxaaShaderProgram->setUniform("enabled", fxaa);
	
	static float minEdgeThreshold = 0.0312f; static float maxEdgeThreshold = 0.125f;
	ImGui::InputFloat("Min edge threshold", &minEdgeThreshold);
	ImGui::InputFloat("Max edge threshold", &maxEdgeThreshold);

	fxaaShaderProgram->setUniform("minEdgeThreshold", minEdgeThreshold);
	fxaaShaderProgram->setUniform("maxEdgeThreshold", maxEdgeThreshold);

	postProcessingBuffer->getColorAttachment(0).bind(GL_TEXTURE0);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();

	fxaaShaderProgram->stopUsing();

	glViewport(0, 0, renderWidth, renderHeight);

}

void renderer::DeferredRenderPipeline::renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget)
{
	
}

GLPBRMaterial* renderer::DeferredRenderPipeline::createMaterial()
{
	return new GLPBRMaterial(defaultAlbedoMap,defaultNormalMap, defaultMetallicMap, defaultRoughnessMap,defaultAOMap);
}
