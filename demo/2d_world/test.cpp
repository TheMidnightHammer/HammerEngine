#include "../../include/HammerEngine/HammerEngine.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

int main() {
    HammerEngine Engine;

    // 1. Configuration
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.texturePath = "textures/texture.png";
    Engine.cameraMode2dOr3d = 1; // Set to 1 for 2D mode if your engine uses this flag
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 16.0f;

    // 2. Initialize Core Vulkan
    Engine.initWindow();
    Engine.initVulkan();

    // 3. Define the single Quad (Front Face)
    std::vector<Vertex> localVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}},
    };

    std::vector<uint32_t> localIndices = {
        0, 1, 2, 2, 3, 0
    };

    // 4. Create Pipeline and Mesh
    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    
    // Create the pipeline (2-sided mode false as per your original)
    auto mainPipeline = std::make_unique<HammerPipeline>(
        Engine, vPath, fPath, 1, true
    );

    // Add to the engine's mesh list
    Engine.meshs.push_back(std::make_unique<HammerMesh>(
        Engine, 
        mainPipeline.get(), 
        localVertices, 
        localIndices
    ));

    // 5. Drawing loop
    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        // Use the 2D Camera logic
        Engine.updateCameraDefault2D();
        
        // Draw all meshes in the list
        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}