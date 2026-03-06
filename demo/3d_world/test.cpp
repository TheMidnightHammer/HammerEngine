/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#include "../../include/HammerEngine/HammerEngine.h"
#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// === Vertex Data (Using your specific UVs and Colors) ===
static const Vertex faceVertices[6][4] = {
    // Top face (+Y)
    {
        {{-0.5f, 0.5f,-0.5f},{1.0f,0.0f,0.0f},{0.1250f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.0f,1.0f,0.0f},{0.1875f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.1875f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.1250f,0.0000f}}
    },
    // Bottom face (-Y)
    {
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.1250f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.5f,0.5f},{0.1250f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.0f},{0.0625f,0.0000f}}
    },
    // Right face (+X)
    {
        {{ 0.5f,-0.5f,-0.5f},{1.0f,0.5f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,1.0f,0.0f},{0.0625f,0.0000f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,1.0f,0.5f},{0.0000f,0.0000f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.0f,1.0f},{0.0000f,0.0625f}}
    },
    // Left face (-X)
    {
        {{-0.5f,-0.5f,-0.5f},{0.5f,0.0f,0.5f},{0.0625f,0.0625f}},
        {{-0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f,0.5f,0.5f},{0.0000f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.5f},{0.0000f,0.0625f}}
    },
    // Front face (+Z)
    {
        {{-0.5f,-0.5f, 0.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}}
    },
    // Back face (-Z)
    {
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.5f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f,-0.5f},{0.0f,0.0f,0.0f},{0.0000f,0.0000f}}
    }
};

static const uint32_t faceIndicesPattern[] = { 0, 1, 2, 2, 3, 0 };

void generateCubeGrid(std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices, int width, int height, int depth) {
    uint32_t vertexOffset = 0;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                glm::vec3 worldPos = glm::vec3((float)x, (float)y, (float)z);
                for (int face = 0; face < 6; ++face) {
                    for (int i = 0; i < 4; ++i) {
                        Vertex v = faceVertices[face][i];
                        v.pos += worldPos;
                        outVertices.push_back(v);
                    }
                    for (int i = 0; i < 6; ++i) {
                        outIndices.push_back(faceIndicesPattern[i] + vertexOffset);
                    }
                    vertexOffset += 4;
                }
            }
        }
    }
}

int main() {
    HammerEngine Engine;

    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1200;
    Engine.WindowHeight = 900;
    Engine.texturePath = "textures/texture.png";
    Engine.mouseLock = 1; 
    Engine.windowName = "Vulkan";
    Engine.renderDistance = 1000.0f;
    Engine.cameraSpeed = 1.0f;

    // --- New Camera Placement for the Floor Grid ---
    // We sit above the grid (Y=40) and look down/forward
    Engine.cameraPosition = glm::vec3(50.0f, 40.0f, 150.0f);
    Engine.cameraFront = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));

    Engine.initWindow();
    Engine.initVulkan();

    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    auto mainPipeline = std::make_unique<HammerPipeline>(Engine, vPath, fPath, 1, true);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    // Grid is now 100 wide (X), 1 high (Y), 100 deep (Z)
    generateCubeGrid(vertices, indices, 1, 1, 1);

    Engine.meshs.push_back(std::make_unique<HammerMesh>(
        Engine, mainPipeline.get(), vertices, indices
    ));

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();
        Engine.updateCameraDefault3D();
        Engine.drawFrame(); 
        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}