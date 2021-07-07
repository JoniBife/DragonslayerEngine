#include "Renderer3D.h"
#include "../utils/OpenGLUtils.h"
#include "../Configurations.h"
#include "../core/MeshRenderer.h"

using namespace core;

core::Renderer3D::Renderer3D()
{

}

core::Renderer3D::~Renderer3D()
{

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

void core::Renderer3D::renderToFrameBuffer(const Camera& camera, Hierarchy& hierarchy, FrameBuffer& frameBuffer)
{
    frameBuffer.bind();

    GL_CALL(glClearColor(BACKGROUND_COLOR));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // we're not using the stencil buffer now
    GL_CALL(glEnable(GL_DEPTH_TEST));

    render(camera, hierarchy);

    frameBuffer.unbind();
}

void core::Renderer3D::render(const Camera& camera, Hierarchy& hierarchy)
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

            material->bindTextures();

            mesh->draw();

            material->unBindTextures();
        }
        mesh->unBind();
        material->stopUsing();
    });
}
