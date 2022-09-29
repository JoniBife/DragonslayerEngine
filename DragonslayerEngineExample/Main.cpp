#include <iostream>
#include <DragonslayerEngine/Renderer.h>
#include <GLFW/glfw3.h>
#include <cameras/EditorCamera.h>
#include <core/Input.h>
#include <DragonslayerEngine/MeshGroup.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>

using namespace DragonslayerEngine;

void glfwWindowSizeCallBack(GLFWwindow* win, int winx, int winy);
void glfwFramebufferSizeCallBack(GLFWwindow* win, int winx, int winy);
void glfwErrorCallback(int error, const char* description);
GLFWwindow* loadGlfwAndCreateWindow(unsigned int width, unsigned int height, const char* title);
void setImGuiStyle();

int main()
{
    RenderingConfigurations renderingConfigs;

    GLFWwindow* window = loadGlfwAndCreateWindow(renderingConfigs.renderWidth, renderingConfigs.renderHeight, "DragonslayerEngine - Example");

    Renderer* renderer = new Renderer(
            renderingConfigs,
            (GLLoadProc)glfwGetProcAddress
    );

    core::Input::initialize(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    setImGuiStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    EditorCamera camera; {
        camera.setEditorWindowFocus(true);
        camera.setViewportSize(renderingConfigs.renderWidth, renderingConfigs.renderHeight);
        camera.setPosition(LMath::Vec3(0.0, 0.0, 3.0));
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
        renderCommand2.material->setRoughness(0.5);
    }

    RenderCommand renderCommand3; {
        renderCommand3.model = Mat4::translation({0.0f, 0.0f, 0.0f}) * Mat4::scaling(2.0f, 10.0f, 2.0f);
        MeshGroup group2 = MeshGroup::loadFromFile("../DragonslayerEngine/assets/objs/cube.obj");
        renderCommand3.mesh = group2.meshes[0];
        renderCommand3.material = renderer->createMaterial();
        renderCommand3.material->setAlbedoTint({ 1.0f, 1.0f, 1.0f });
    }

    RenderCommand renderCommand4; {
        MeshGroup group2 = MeshGroup::loadFromFile("assets/Dragonslayer/dragonslayer.obj");
        renderCommand4.mesh = group2.meshes[0];
        renderCommand4.material = renderer->createMaterial();
        renderCommand4.material->setAlbedoMap(new Texture2D("assets/Dragonslayer/Dragon_slayer_albedo.jpeg"));
        renderCommand4.material->setAOMap(new Texture2D("assets/Dragonslayer/Dragon_slayer_AO.jpeg"));
        renderCommand4.material->setMetallicMap(new Texture2D("assets/Dragonslayer/Dragon_slayer_metallic.jpeg"));
        renderCommand4.material->setRoughnessMap(new Texture2D("assets/Dragonslayer/Dragon_slayer_roughness.jpeg"));
        renderCommand4.material->setNormalMap(new Texture2D("assets/Dragonslayer/Dragon_slayer_normal.png"));
    }

    MeshGroup group = MeshGroup::loadFromFile("../DragonslayerEngine/assets/objs/sphere.obj");
    std::vector<RenderCommand> renderCommandSpheres;

    float p = 0.0f;
    for (int i = 0; i < 30; ++i) {
        RenderCommand renderCommandSphere;

        renderCommandSphere.model = Mat4::translation(0.0, 0.0, -p * 4.0f);
        std::cout << renderCommandSphere.model << std::endl;

        renderCommandSphere.mesh = group.meshes[0];
        renderCommandSphere.material = renderer->createMaterial();
        renderCommandSphere.material->setAlbedoTint({1.0f, 1.0f, 1.0f});

        renderCommandSpheres.push_back(renderCommandSphere);
        p += 1.0f;
    }

    ACESToneMappingCommand toneMapping = ACESToneMappingCommand();
    //ReinhardToneMappingCommand toneMapping = ReinhardToneMappingCommand();
    FxaaCommand fxaa = FxaaCommand();

    double elapsedTime, lastTime = 0.0;
    bool show_demo_window = true;
    bool showStatistics = true;
    bool isKeyOneDown = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        double time = glfwGetTime();
        elapsedTime = time - lastTime;
        lastTime = time;

        if (core::Input::isKeyDown(core::KeyCode::ONE) && !isKeyOneDown) {
            showStatistics = !showStatistics;
            isKeyOneDown = true;
        } else if (core::Input::isKeyUp(core::KeyCode::ONE) && isKeyOneDown) {
            isKeyOneDown = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        if (showStatistics)
        {
            ImGui::Begin("Statistics", &showStatistics);
            ImGui::Text("Geometry pass: %f", renderer->getFrameTime(RenderPass::GEOMETRY));
            ImGui::Text("Light pass: %f", renderer->getFrameTime(RenderPass::LIGHT));
            ImGui::Text("Shadow pass: %f", renderer->getFrameTime(RenderPass::SHADOW));
            ImGui::Text("Post-processing pass: %f", renderer->getFrameTime(RenderPass::POSTPROCESSING));
            ImGui::Text("Ssao pass: %f", renderer->getFrameTime(RenderPass::SSAO));
            ImGui::Text("Delta time : %f", elapsedTime * 1000.0);
            ImGui::Text("Framerate : %d", static_cast<unsigned int>(ImGui::GetIO().Framerate));
            ImGui::End();
        }

        camera.update(elapsedTime);
        renderCommand.model = Mat4::translation(cos(time) * 10.0, sin(time * 7.0) * 3.0 + 5.0, 0.0);
        renderCommand.material->setMetallic(sin(time) * 0.5 + 0.5);
        renderCommand.material->setRoughness(sin(time * .5) * 0.5 + 0.5);
        renderer->enqueuePostProcessing(&fxaa);
        renderer->enqueuePostProcessing(&toneMapping);
        //renderer->enqueueRender(&renderCommand);
        renderer->enqueueRender(&renderCommand2);
        //renderer->enqueueRender(&renderCommand3);

        renderCommand4.model = Mat4::rotation(sin(time), Vec3::Z) * Mat4::rotation(cos(time), Vec3::Y)  * Mat4::rotation(cos(time), Vec3::X)  * Mat4::scaling(1.0f);
        renderer->enqueueRender(&renderCommand4);

        for (RenderCommand& rcSphere : renderCommandSpheres) {
            renderer->enqueueRender(&rcSphere);
        }

        renderer->render(camera, lights);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete renderer;

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

    GLFWwindow* window = glfwCreateWindow(width, height, title, 0, nullptr);
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

void setImGuiStyle() {

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.16f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.75f, 0.26f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.98f, 0.75f, 0.26f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.88f, 0.63f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
