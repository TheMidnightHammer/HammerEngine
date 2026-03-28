/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#include "../../include/HammerEngine/HammerEngine.h"
#include "../../include/HammerEngine/HammerRect.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include <string>
#include <glm/glm.hpp>

int main() {
    HammerEngine Engine;

    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.MaxTextures = 1000;
    Engine.cameraPosition = glm::vec3(0, 0, 2);
    Engine.windowName = "Hammer Engine - Sprite Movement";
    Engine.mouseLock = 0;
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 16.0f;
    Engine.setMaxVertciesIndicesSize(1024 * 1024);

    Engine.initWindow();
    Engine.initVulkan();

    auto mainTexture = std::make_unique<HammerTexture>(Engine, "textures/texture.png", HammerTextureFilter::Nearest);

    HammerRectSquareF player{0, 2, 1, 1};

    auto getVertices = [&](const HammerRectSquareF& p) {
        return std::vector<Vertex>{
            {{-0.5f, -0.5f, -1.5f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
            {{ 0.5f, -0.5f, -1.5f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
            {{ 0.5f,  0.5f, -1.5f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
            {{-0.5f,  0.5f, -1.5f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}}
        };
    };

    std::vector<uint32_t> localIndices = { 0, 1, 2, 2, 3, 0 };

    std::string vShader = "shaders/vert.spv";
    std::string fShader = "shaders/frag.spv";

    auto mainPipeline = std::make_unique<HammerPipeline>(Engine, vShader, fShader, 1, true);
    
    auto sceneMesh = std::make_unique<HammerMesh>(Engine, mainPipeline.get(), mainTexture.get(), getVertices(player), localIndices);

    HammerMesh* meshPtr = sceneMesh.get();
    Engine.meshs.push_back(std::move(sceneMesh));

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        bool moved = false;
        if (glfwGetKey(Engine.window, GLFW_KEY_I) == GLFW_PRESS) { player.y += 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_K) == GLFW_PRESS) { player.y -= 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_J) == GLFW_PRESS) { player.x -= 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_L) == GLFW_PRESS) { player.x += 0.1f; moved = true; }

        if (moved) {
            meshPtr->position = glm::vec3(player.x, player.y, 0);
        }

        Engine.updateCameraDefault3D();
        Engine.drawFrame();
        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    mainTexture.reset();
    mainPipeline.reset();

    return EXIT_SUCCESS;
}