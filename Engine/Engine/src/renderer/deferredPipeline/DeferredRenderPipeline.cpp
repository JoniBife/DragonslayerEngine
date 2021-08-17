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

renderer::DeferredRenderPipeline::DeferredRenderPipeline() : RenderPipeline(new DeferredRenderQueue())
{

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



	// 4. Creation of intermediate framebuffers
	FrameBufferBuilder frameBufferBuilder;
	
	gBuffer = frameBufferBuilder
		.setSize(1,1)
		.attachColorBuffers(4, GL_FLOAT)
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

}

void renderer::DeferredRenderPipeline::render(const Camera& camera, const Lights& lights)
{

}

void renderer::DeferredRenderPipeline::renderToFramebuffer(const Camera& camera, const Lights& lights, const FrameBuffer& frameBuffer)
{

}
