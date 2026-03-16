/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#include "../../include/HammerEngine/HammerEngine.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

int main() {
    HammerEngine Engine;

    // --- Engine Configuration ---
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.windowName = "Hammer Engine - 2D Texture Demo";
    Engine.MaxTextures = 1000; // Ensures the Descriptor Pool has enough gas
    Engine.mouseLock = 0;
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 16.0f;

    // --- Initialization ---
    Engine.initWindow();
    Engine.initVulkan();

    // 1. Create the Pipeline
    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    auto mainPipeline = std::make_unique<HammerPipeline>(
        Engine, vPath, fPath, 1, true
    );

    // 2. Load the Texture (NEW)
    // We create the texture object after initVulkan so the Device exists
    auto mainTexture = std::make_unique<HammerTexture>(
        Engine, "textures/texture.png", HammerTextureFilter::Nearest
    );

    // --- Geometry Data ---
    std::vector<Vertex> localVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}},
    };

    std::vector<uint32_t> localIndices = {
        0, 1, 2, 2, 3, 0
    };

    // 3. Create Mesh using the Pipeline AND Texture
    // The constructor now receives mainTexture.get()
    Engine.meshs.push_back(std::make_unique<HammerMesh>(
        Engine, 
        mainPipeline.get(), 
        mainTexture.get(), 
        localVertices, 
        localIndices
    ));

    // --- Main Loop ---
    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        Engine.updateCameraDefault2D();
        
        // This will call recordCommandBuffer, which now binds Set 1 (Texture)
        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    // --- Cleanup ---
    // Explicitly destroy Vulkan resources before the engine shuts down
    mainTexture.reset(); 
    mainPipeline.reset();

    return EXIT_SUCCESS;
}