#include <iostream>
#include <DragonslayerEngine/Renderer.h>
#include <GLFW/glfw3.h>
#include <cameras/EditorCamera.h>
#include <core/Input.h>
#include <DragonslayerEngine/MeshGroup.h>

using namespace DragonslayerEngine;

void glfwWindowSizeCallBack(GLFWwindow* win, int winx, int winy);
void glfwFramebufferSizeCallBack(GLFWwindow* win, int winx, int winy);
void glfwErrorCallback(int error, const char* description);
GLFWwindow* loadGlfwAndCreateWindow(unsigned int width, unsigned int height, const char* title);

int main()
{
    RenderingConfigurations renderingConfigs;

    GLFWwindow* window = loadGlfwAndCreateWindow(renderingConfigs.renderWidth, renderingConfigs.renderHeight, "DragonslayerEngine - Example");

    Renderer* renderer = new Renderer(
            renderingConfigs,
            (GLLoadProc)glfwGetProcAddress
    );

    core::Input::initialize(window);

    EditorCamera camera; {
        camera.setEditorWindowFocus(true);
        camera.setViewportSize(renderingConfigs.renderWidth, renderingConfigs.renderHeight);
        camera.setPosition(LMath::Vec3(0.0, 10.0, 20.0));
        camera.setFarPlane(200.0);
    }

    Lights lights; {
        DirectionalLight directionalLight; {
            directionalLight.intensity = 1.0f;
            directionalLight.color = LMath::Vec3(1.0, 1.0, 1.0);
        }
        lights.directionalLights.emplace_back(directionalLight);
    }

    RenderCommand renderCommand; {
        renderCommand.model = Mat4::scaling(1.0f);
        MeshGroup group = MeshGroup::loadFromFile("../DragonslayerEngine/assets/objs/sphere.obj");
        renderCommand.mesh = group.meshes[0];
        renderCommand.material = renderer->createMaterial();
        renderCommand.material->setAlbedoTint({1.0f, 1.0f, 1.0f});
    }

    RenderCommand renderCommand2; {
        renderCommand2.model = Mat4::translation({0.0f, -1.0f, 0.0f});
        MeshGroup group2 = MeshGroup::loadFromFile("../DragonslayerEngine/assets/objs/plane.obj");
        renderCommand2.mesh = group2.meshes[0];
        renderCommand2.material = renderer->createMaterial();
        renderCommand2.material->setAlbedoTint({ 1.0f, 1.0f, 1.0f });
    }

    ACESToneMappingCommand toneMapping = ACESToneMappingCommand();
    //ReinhardToneMappingCommand toneMapping = ReinhardToneMappingCommand();
    FxaaCommand fxaa = FxaaCommand();

    double elapsedTime, lastTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        double time = glfwGetTime();
        elapsedTime = time - lastTime;
        lastTime = time;

        renderCommand.material->setMetallic(sin(time) * 0.5 + 0.5);
        renderCommand.material->setRoughness(sin(time * .5) * 0.5 + 0.5);

        camera.update(elapsedTime);
        renderer->enqueuePostProcessing(&fxaa);
        renderer->enqueuePostProcessing(&toneMapping);
        renderer->enqueueRender(&renderCommand);
        renderer->enqueueRender(&renderCommand2);

        renderer->render(camera, lights);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void glfwWindowSizeCallBack(GLFWwindow* win, int winx, int winy)
{
    /*int newWidth, newHeight;
    glfwGetFramebufferSize(win, &newWidth, &newHeight);
    engine->updateWindow(newWidth, newHeight);
    GL_CALL(glViewport(0, 0, newWidth, newHeight));*/
}
void glfwFramebufferSizeCallBack(GLFWwindow* win, int winx, int winy)
{
    //engine->updateWindow(2.0f/3.0f * winx, 2.0f / 3.0f * winy);
    //GL_CALL(glViewport(0, winy / 3.0f, 2.0f / 3.0f * winx , 2.0f/3.0f * winy));
}
void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
}
GLFWwindow* loadGlfwAndCreateWindow(unsigned int width, unsigned int height, const char* title) {

    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
#ifdef _DEBUF_
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, 0, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(true);

    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallBack);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallBack);

    return window;
}