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

    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.windowName = "Hammer Engine - 2D Texture Demo";
    Engine.MaxTextures = 1000;
    Engine.mouseLock = 0;
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 16.0f;


    Engine.initWindow();
    Engine.initVulkan();

    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    auto mainPipeline = std::make_unique<HammerPipeline>(
        Engine, vPath, fPath, 1, true
    );

    auto mainTexture = std::make_unique<HammerTexture>(
        Engine, "textures/texture.png", HammerTextureFilter::Nearest
    );

    std::vector<Vertex> localVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}},
    };

    std::vector<uint32_t> localIndices = {
        0, 1, 2, 2, 3, 0
    };

    Engine.meshs.push_back(std::make_unique<HammerMesh>(
        Engine, 
        mainPipeline.get(), 
        mainTexture.get(), 
        localVertices, 
        localIndices
    ));

    // --- Main Loop ---    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        Engine.updateCameraDefault2D();
        
        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();
    mainTexture.reset(); 
    mainPipeline.reset();

    return EXIT_SUCCESS;
}