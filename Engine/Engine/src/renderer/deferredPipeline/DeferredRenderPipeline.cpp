#include <GL/glew.h>
#include "DeferredRenderPipeline.h"
#include "DeferredRenderQueue.h"
#include "../../utils/OpenGLUtils.h"
#include "GLPBRMaterial.h"
#include "../view/Transformations.h"
#include <imgui/imgui.h>
#include "../../gui/ImGuiExtensions.h"
#include "../../math/MathAux.h"

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
	Shader prostProcessingFS(GL_FRAGMENT_SHADER, "../Engine/shaders/deferred/postProcessingFS.glsl");
	postProcessingShaderProgram = new ShaderProgram(postProcessingVS, prostProcessingFS);

	// 4. Creating intermediate framebuffers and global unifom buffer
	FrameBufferBuilder frameBufferBuilder;

	gBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(3, GL_FLOAT)
		.attachDepthBuffer()
		.attachStencilBuffer()
		.build();

	for (int i = 0; i < maxShadowMaps; ++i) {
		// TODO ShadowMap depth texture has different properties
		shadowMapBuffers.push_back(frameBufferBuilder
			.setSize(1024, 1024)
			.attachColorBuffers(1, GL_HALF_FLOAT)
			.attachDepthBuffer()
			.build());
	}

	lightBuffer = frameBufferBuilder
		.setSize(renderWidth, renderHeight)
		.attachColorBuffers(1, GL_HALF_FLOAT) // Attaching color buffers with GL_RGBA16F precision to avoid clamping to [0,1]
		.attachDepthBuffer()
		.attachStencilBuffer()
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

	createGlobalUniformsBuffer();

	// 5. Creating the quad whose vertices will be used in most render passes
	quadNDC = Mesh::rectangle(2.0f, 2.0f);
	quadNDC->init();

	// 6. Load default material textures
	defaultAlbedoMap = new Texture2D("../Engine/textures/pbr/default/albedo.png");
	defaultNormalMap = new Texture2D("../Engine/textures/pbr/default/normal.png");
	defaultMetallicMap = new Texture2D("../Engine/textures/pbr/default/metallic.png");
	defaultRoughnessMap = new Texture2D("../Engine/textures/pbr/default/roughness.png");
	defaultAOMap = new Texture2D("../Engine/textures/pbr/default/ao.png");
}

renderer::DeferredRenderPipeline::~DeferredRenderPipeline()
{
	delete openGLState;

	// Delete shader programs
	delete geometryShaderProgram;
	delete shadowMapShaderProgram;
	delete pbrShaderProgram;
	delete postProcessingShaderProgram;
	
	// Delete buffers
	delete gBuffer;
	for (int i = 0; i < maxShadowMaps; ++i) {
		delete shadowMapBuffers[i];
	}
	delete prePostProcessingBuffer;
	delete postProcessingBuffer;
}

void renderer::DeferredRenderPipeline::render(const Camera& camera, const Lights& lights)
{
	GL_CALL(glClearColor(0, 0, 0, 1));

	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	openGLState->setViewPort(0, 0, renderWidth, renderHeight);
	openGLState->setDepthTesting(true);
	openGLState->setDepthFunction(GL_LESS);
	//openGLState->setDepthRange(0.0, 1.0);
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
	
	Mat4 lightViewProjection;

	static float left = -20.0f;
	static float right = 20.0f;
	static float bottom = -20.0f;
	static float top = 20.0f;
	static float near = -15.0f;
	static float far = 20.0f;

	ImGui::Text("Light Orthographic Projection Settings");
	ImGui::InputFloat("Left", &left);
	ImGui::InputFloat("Right", &right);
	ImGui::InputFloat("bottom", &bottom);
	ImGui::InputFloat("top", &top);
	ImGui::InputFloat("near", &near);
	ImGui::InputFloat("far", &far);

	Mat4 projection = ortho(left, right, bottom, top, near, far);

	if (shadowMapCommands.size() > 0) {
		
		// TODO this should not be hardcoded
		openGLState->setViewPort(0, 0, 1024, 1024);
		//openGLState->setCullFace(GL_FRONT);
		//openGLState->setFaceCulling(false);

		for (int i = 0; i < lights.directionalLights.size(); ++i) {

			shadowMapBuffers[i]->bind();
			GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

			shadowMapShaderProgram->use();
			Mat4 lightView = lookAt(-1 * lights.directionalLights[i].direction * 10.0f, Vec3::ZERO, Vec3::Y);

			Mat4 inverseCameraView;
			camera.getView().inverse(inverseCameraView);
			Mat4 cascadeProjection = orthoCascade(
				0.5f, 
				7.0f, 
				degreesToRadians(camera.getFov()), 
				camera.getViewportHeight() / camera.getViewportWidth(), 
				inverseCameraView, 
				lightView);
			
			lightViewProjection = projection * lightView;

			shadowMapShaderProgram->setUniform("lightSpaceProjectionMatrix", lightViewProjection);
			
			for (const RenderCommand shadowMapCommand : shadowMapCommands._Get_container()) {

				shadowMapShaderProgram->setUniform("modelMatrix", shadowMapCommand.model);
				shadowMapCommand.mesh->bind();
				shadowMapCommand.mesh->draw();
				shadowMapCommand.mesh->unBind();
			}

			shadowMapShaderProgram->stopUsing();
			shadowMapBuffers[i]->unbind();

			ImGui::Image((ImTextureID)shadowMapBuffers[i]->getColorAttachment(0).getId(), ImVec2(400,400), ImVec2(0, 1), ImVec2(1, 0));

			
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
	
	//lightBuffer->drawBuffers();
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); // Clearing all buffer attachments, MUST be done after drawBuffers
	pbrShaderProgram->use();

	pbrShaderProgram->setUniform("lightSpaceMatrix", lightViewProjection);
	pbrShaderProgram->setUniform("viewPosition", camera.getPosition());
	pbrShaderProgram->setUniform("gBufferPositionMetallic", 0);
	pbrShaderProgram->setUniform("gBufferNormalRoughness", 1);
	pbrShaderProgram->setUniform("gBufferAlbedoAmbientOcclusion", 2);
	pbrShaderProgram->setUniform("shadowMap", 3);
	static Vec3 pixelOffset;
	ImGui::InputVec3("Pixel Offset", pixelOffset);
	pbrShaderProgram->setUniform("pixelOffset", pixelOffset);

	gBuffer->getColorAttachment(0).bind(GL_TEXTURE0);
	gBuffer->getColorAttachment(1).bind(GL_TEXTURE1);
	gBuffer->getColorAttachment(2).bind(GL_TEXTURE2);
	shadowMapBuffers[0]->getDepthAttachment().bind(GL_TEXTURE3);

	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();

	pbrShaderProgram->stopUsing();
	
	lightBuffer->unbind();

	//glViewport(0,0,renderWidth, renderHeight);

	// 3. Apply any post processing
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	postProcessingShaderProgram->use();
	postProcessingShaderProgram->setUniform("previousRenderTexture",0);
	lightBuffer->getColorAttachment(0).bind(GL_TEXTURE0);
	//shadowMapBuffers[0]->getDepthAttachment().bind(GL_TEXTURE0);
	quadNDC->bind();
	quadNDC->draw();
	quadNDC->unBind();
	postProcessingShaderProgram->stopUsing();
}

void renderer::DeferredRenderPipeline::renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget)
{
	
}

GLPBRMaterial* renderer::DeferredRenderPipeline::createMaterial()
{
	return new GLPBRMaterial(defaultAlbedoMap,defaultNormalMap, defaultMetallicMap, defaultRoughnessMap,defaultAOMap);
}
