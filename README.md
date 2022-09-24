# Dragonslayer Engine

A simple 3D rendering engine that can be easily integrated in any c++ application.
Currently supports OpenGL only. Even through the engine was designed to be integrated in a game engine. It can be used by any application that required 3D rendering.

## Using the engine
```C++
// 1. Create rendering configurations
RenderingConfigurations renderingConfigs; {
    renderingConfigs.shadowResolution = 1024u;
    // change other configs
}

// 2. Create the renderer object from the rendering configurations
Renderer* renderer = new Renderer(renderingConfigs,(GLLoadProc)glfwGetProcAddress);

// 3. Create a camera
EditorCamera camera; {
    camera.setViewportSize(renderingConfigs.renderWidth, renderingConfigs.renderHeight);
    camera.setPosition(Vec3(0.0, 10.0, 20.0));
    camera.setFarPlane(200.0);
    // Change other camera settings
}

// 4. Create lights
Lights lights; {
    DirectionalLight directionalLight; {
        directionalLight.intensity = 1.0f;
        directionalLight.color = Vec3(1.0, 1.0, 1.0);
        lights.directionalLights.emplace_back(directionalLight);
        // Change other directional light settings
    }
    // Add more lights
}

// 5. Create rendering commands
RenderCommand renderSphereCommand; {
    renderCommand.model = Mat4::scaling(1.0f);
    MeshGroup group = MeshGroup::loadFromFile("../DragonslayerEngine/assets/objs/sphere.obj");
    renderCommand.mesh = group.meshes[0];
    renderCommand.material = renderer->createMaterial();
    renderCommand.material->setAlbedoTint({1.0f, 1.0f, 1.0f});
    // Configure other PBR material properties
}

// 6. Create post-processing commands 
ACESToneMappingCommand toneMapping = ACESToneMappingCommand();
FxaaCommand fxaa = FxaaCommand();

while (//window loop)
{
    // 7. Update camera        
    camera.update(deltaTime);
    
    // 8. Enqueue rendering and post-processing commands
    renderer->enqueueRender(&renderSphereCommand);
    renderer->enqueuePostProcessing(&fxaa);
    renderer->enqueuePostProcessing(&toneMapping);
    
    // 9. Render scene :)
    renderer->render(camera, lights);
    
    // swap buffers
}

// 10. Delete engine once app closes
delete renderer;

return 0;
```

## Building from source

- Clone the repository and its git submodules
- Generate makefile using CMake
- Build project



