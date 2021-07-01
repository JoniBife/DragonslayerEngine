#include "Renderer3D.h"
#include "../utils/OpenGLUtils.h"
#include "../Configurations.h"
#include "../core/MeshRenderer.h"

core::Renderer3D::Renderer3D()
{

}

core::Renderer3D::~Renderer3D()
{
    if (frameTexture != nullptr && frameBuffer != nullptr) {
        delete frameBuffer;
        delete frameTexture;
    }


}

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

void core::Renderer3D::setup()
{
#if _DEBUG
    printOpenGLInfo();
#endif
    GL_CALL(glClearColor(BACKGROUND_COLOR));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glDepthFunc(GL_LEQUAL));
    GL_CALL(glDepthMask(GL_TRUE));
    GL_CALL(glDepthRange(0.0, 1.0));
    GL_CALL(glClearDepth(1.0));
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glCullFace(GL_BACK));
    GL_CALL(glFrontFace(GL_CCW));
    if (MSAA > 0) GL_CALL(glEnable(GL_MULTISAMPLE));
    GL_CALL(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
}

Texture2D& core::Renderer3D::RenderToTexture(const Camera& camera, Hierarchy& hierarchy)
{
    if (frameTexture == nullptr && frameBuffer == nullptr) {
        frameTexture = Texture2D::emptyTexture(camera.getViewportWidth(), camera.getViewportHeight());
        frameBuffer = new FrameBuffer([&]() {

            // Bind frameBuffer

            frameTexture->bind(GL_TEXTURE0);
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture->getId(), 0));
            frameTexture->unBind(GL_TEXTURE0);

            GL_CALL(glGenRenderbuffers(1, &rbo));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
            GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera.getViewportWidth(), camera.getViewportHeight()));
            GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

            GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

            // Unbind frameBuffer
        });
    } 
    
    // TODO Consider resizing the the texture and renderbuffer sizes when window changes
    // Currently not working properly

    /*else if (camera.getViewportWidth() != frameTexture->getWidth() ||
        camera.getViewportHeight() != frameTexture->getHeight()) {

        // The camera width and height were changed so we resize both the texture
        // and framebuffer

        frameTexture->resize(camera.getViewportWidth(), camera.getViewportHeight());

        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera.getViewportWidth(), camera.getViewportHeight()));
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }*/

    frameBuffer->bind();

    GL_CALL(glClearColor(BACKGROUND_COLOR));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // we're not using the stencil buffer now
    GL_CALL(glEnable(GL_DEPTH_TEST));

    Render(camera, hierarchy);

    frameBuffer->unbind();

    return *frameTexture;
}

void core::Renderer3D::Render(const Camera& camera, Hierarchy& hierarchy)
{
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    hierarchy.traverseScene([&](GameObject* gameObject) {

        MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();

        if (meshRenderer == nullptr)
            return;

        mesh = meshRenderer->getMesh();
        material = meshRenderer->getMaterial();

        // Cannot render the mesh if the mesh or the material are not set
        if (mesh == nullptr || material == nullptr)
            return;

        material->use();
        mesh->bind();
        {
            Transform* transform = meshRenderer->getGameObject()->getTransform();

            Mat3 inverse;
            bool canInverse = transform->getModelMatrix().toMat3().inverse(inverse);

            if (canInverse)
                material->sendParametersToShader(camera.getPosition(), transform->getModelMatrix(), inverse.transpose());
            else
                material->sendParametersToShader(camera.getPosition(), transform->getModelMatrix());

            mesh->draw();
        }
        mesh->unBind();
        material->stopUsing();
    });
}
