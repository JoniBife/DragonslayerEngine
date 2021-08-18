#include "DeferredRenderPipeline.h"
#include "DeferredRenderQueue.h"
#include <GL/glew.h>
#include "../../utils/OpenGLUtils.h"

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

DeferredRenderPipeline* renderer::DeferredRenderPipeline::create()
{
	return new DeferredRenderPipeline(new DeferredRenderQueue());
}

renderer::DeferredRenderPipeline::DeferredRenderPipeline(DeferredRenderQueue* deferredRenderQueue) {
	this->deferredRenderQueue = deferredRenderQueue;

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

	// 4. Creating intermediate framebuffers
	FrameBufferBuilder frameBufferBuilder;

	gBuffer = frameBufferBuilder
		.setSize(1, 1)
		.attachColorBuffers(3, GL_FLOAT)
		.attachDepthBuffer()
		.attachStencilBuffer()
		.build();

	for (int i = 0; i < maxShadowMaps; ++i) {
		// TODO ShadowMap depth texture has different properties
		shadowMapBuffers.push_back(frameBufferBuilder
			.setSize(1, 1)
			.attachDepthBuffer()
			.build());
	}

	prePostProcessingBuffer = frameBufferBuilder
		.setSize(1, 1)
		.attachColorBuffers(1, GL_HALF_FLOAT)
		.attachDepthBuffer()
		.attachStencilBuffer()
		.build();

	postProcessingBuffer = frameBufferBuilder
		.setSize(1, 1)
		.attachColorBuffers(1, GL_UNSIGNED_BYTE)
		.build();
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
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	openGLState->setViewPort(0, 0, renderWidth, renderHeight);
	openGLState->setDepthTesting(true);
	openGLState->setDepthFunction(GL_LESS);
	openGLState->setDepthRange(0.0, 1.0);
	openGLState->setFaceCulling(true);
	openGLState->setCullFace(GL_BACK);
	openGLState->setFrontFace(GL_CCW);
	openGLState->setBlending(false);

	// 1. Render all the geometry to the gBuffer
	gBuffer->bind();
	gBuffer->drawBuffers();
	geometryShaderProgram->use();

	RenderCommand renderCommand;
	while (!deferredRenderQueue->isGeometryEmpty()) {
		renderCommand = deferredRenderQueue->dequeueGeometry();

		renderCommand.material;

		renderCommand.mesh->draw();
	}

	geometryShaderProgram->stopUsing();
	gBuffer->unbind();


	// 2. Render from each of the lights perspective to generate each of the shadow maps



	// 3. Apply any post processing
	postProcessingShaderProgram->use();
	
	postProcessingShaderProgram->stopUsing();

}

void renderer::DeferredRenderPipeline::renderToTarget(const Camera& camera, const Lights& lights, const RenderTarget& renderTarget)
{
	
}
