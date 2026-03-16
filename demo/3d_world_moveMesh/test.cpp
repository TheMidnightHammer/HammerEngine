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

// ... (faceVertices and faceIndicesPattern remain unchanged) ...

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

    // --- Configuration ---
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1200;
    Engine.WindowHeight = 900;
    Engine.MaxTextures = 1000; // This ensures the Descriptor Pool is large enough
    Engine.mouseLock = 1; 
    Engine.windowName = "Hammer Engine - 3D Texture World";
    Engine.renderDistance = 1000.0f;
    Engine.cameraSpeed = 2.5f;

    Engine.cameraPosition = glm::vec3(5.0f, 5.0f, 15.0f);
    Engine.cameraFront = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));

    // --- Initialization ---
    Engine.initWindow();
    Engine.initVulkan();

    // 1. Create the Pipeline
    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    auto mainPipeline = std::make_unique<HammerPipeline>(Engine, vPath, fPath, 1, true);

    // 2. Load Textures (Must happen after initVulkan)
    // We use Linear filtering for a smooth look
    auto boxTexture = std::make_unique<HammerTexture>(Engine, "textures/texture.png", HammerTextureFilter::Nearest);

    // 3. Prepare Geometry
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    generateCubeGrid(vertices, indices, 10, 10, 10);

    // 4. Create Mesh and link to the texture
    // Updated constructor now takes the texture pointer
    auto myMesh = std::make_unique<HammerMesh>(Engine, mainPipeline.get(), boxTexture.get(), vertices, indices);
    
    HammerMesh* meshPtr = myMesh.get();
    Engine.meshs.push_back(std::move(myMesh));

    // --- Main Loop ---
    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        float time = static_cast<float>(glfwGetTime());

        // Simple animation
        meshPtr->position.y = sin(time) * 0.5f; 
        meshPtr->rotation.y = time * 20.0f;

        Engine.updateCameraDefault3D();
        
        // drawFrame calls recordCommandBuffer, which now binds the texture sets
        Engine.drawFrame(); 
        
        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    // --- Cleanup ---
    // Destroy textures explicitly before the Engine shuts down Vulkan
    boxTexture.reset();
    mainPipeline.reset();

    return EXIT_SUCCESS;
}