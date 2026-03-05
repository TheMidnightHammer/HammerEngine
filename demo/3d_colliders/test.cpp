/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../../include/HammerEngine/HammerEngine.h"
#include "../../include/HammerEngine/HammerRect.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

int main() {
    HammerEngine Engine;

    // 1. Initial Configuration
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 900;
    Engine.WindowHeight = 900;
    Engine.texturePath = "textures/texture.png";
    Engine.mouseLock = 1;
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 64.0f;
    Engine.cameraPosition = glm::vec3(0, 5, 0);

    // 2. Initialize Core Vulkan
    Engine.initWindow();
    Engine.initVulkan();

    // 3. Define the cube data locally
    std::vector<Vertex> localVertices = {
        // Front (tile 0,0)
        {{-0.5f,-0.5f, 0.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},

        // Back (tile 0,0)
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.5f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f,-0.5f},{0.0f,0.0f,0.0f},{0.0000f,0.0000f}},

        // Top (tile 2,0)
        {{-0.5f, 0.5f,-0.5f},{1.0f,0.0f,0.0f},{0.1250f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.0f,1.0f,0.0f},{0.1875f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.1875f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.1250f,0.0000f}},

        // Bottom (tile 1,0)
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.1250f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.5f,0.5f},{0.1250f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.0f},{0.0625f,0.0000f}},

        // Right (tile 0,0)
        {{ 0.5f,-0.5f,-0.5f},{1.0f,0.5f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,1.0f,0.0f},{0.0625f,0.0000f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,1.0f,0.5f},{0.0000f,0.0000f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.0f,1.0f},{0.0000f,0.0625f}},

        // Left (tile 0,0)
        {{-0.5f,-0.5f,-0.5f},{0.5f,0.0f,0.5f},{0.0625f,0.0625f}},
        {{-0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f,0.5f,0.5f},{0.0000f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.5f},{0.0000f,0.0625f}}
    };

    std::vector<uint32_t> localIndices = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Top
        12, 13, 14, 14, 15, 12, // Bottom
        16, 17, 18, 18, 19, 16, // Right
        20, 21, 22, 22, 23, 20  // Left
    };

    // 4. Create Pipeline and Mesh
    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    
    // We create the pipeline as a unique_ptr to manage its lifetime
    auto mainPipeline = std::make_unique<HammerPipeline>(
        Engine, vPath, fPath, 1, true
    );

    // Add the cube to the engine's mesh list
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

        // Collision logic
        HammerRectCubeF cube{0, 0, 0, 1, 1, 1};
        HammerRectCubeF camera{
            Engine.cameraPosition.x,
            Engine.cameraPosition.y - 1.0f,
            Engine.cameraPosition.z, 
            1, 1, 1
        };

        bool isColliding = camera.HammerRectCollideCubeF(cube);
        
        // Update gravity based on collision
        Engine.updateCameraDefaultGravety3D(isColliding);
        
        // The Engine now automatically loops through Engine.meshs in drawFrame
        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}