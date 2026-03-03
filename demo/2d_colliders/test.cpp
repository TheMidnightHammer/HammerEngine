#include "../../include/HammerEngine/HammerEngine.h"
#include "../../include/HammerEngine/HammerRect.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

int main() {
    HammerEngine Engine;

    // 1. Setup
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.texturePath = "textures/texture.png";
    Engine.cameraPosition = glm::vec3(0, 0, 2);
    Engine.cameraMode2dOr3d = 0;
    Engine.cameraSpeed = 0.1f;
    Engine.renderDistance = 16.0f;

    Engine.initWindow();
    Engine.initVulkan();

    // 2. Logic state
    HammerRectSquareF square{0, 0, 1, 1};
    HammerRectSquareF player{0, 2, 1, 1};

    // 3. Initial Mesh Data
    auto getVertices = [&](const HammerRectSquareF& p) {
        return std::vector<Vertex>{
            // Player quad (Front)
            {{p.x - 0.5f, p.y - 0.5f, -1.5f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
            {{p.x + 0.5f, p.y - 0.5f, -1.5f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
            {{p.x + 0.5f, p.y + 0.5f, -1.5f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
            {{p.x - 0.5f, p.y + 0.5f, -1.5f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}},

            // Cube quad (Static-ish)
            {{-0.5f, -0.5f, -1.5f}, {1.0f, 0.0f, 0.0f}, {0.0000f, 0.0625f}},
            {{ 0.5f, -0.5f, -1.5f}, {0.0f, 1.0f, 0.0f}, {0.0625f, 0.0625f}},
            {{ 0.5f,  0.5f, -1.5f}, {0.0f, 0.0f, 1.0f}, {0.0625f, 0.0000f}},
            {{-0.5f,  0.5f, -1.5f}, {1.0f, 1.0f, 0.0f}, {0.0000f, 0.0000f}},
        };
    };

    std::vector<uint32_t> localIndices = {
        0, 1, 2, 2, 3, 0, // Player
        4, 5, 6, 6, 7, 4  // Cube
    };

    // 4. Create Pipeline and initial Mesh
    auto mainPipeline = std::make_unique<HammerPipeline>(Engine, "shaders/vert.spv", "shaders/frag.spv", 0, true);
    
    // We keep a pointer to the mesh so we can update it in the loop
    auto sceneMesh = std::make_unique<HammerMesh>(Engine, mainPipeline.get(), getVertices(player), localIndices);
    HammerMesh* meshPtr = sceneMesh.get();
    
    Engine.meshs.push_back(std::move(sceneMesh));

    // 5. Main Loop
    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        bool moved = false;
        if (glfwGetKey(Engine.window, GLFW_KEY_I) == GLFW_PRESS) { player.y += 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_K) == GLFW_PRESS) { player.y -= 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_J) == GLFW_PRESS) { player.x -= 0.1f; moved = true; }
        if (glfwGetKey(Engine.window, GLFW_KEY_L) == GLFW_PRESS) { player.x += 0.1f; moved = true; }

        // Only update the GPU buffers if the player actually moved
        if (moved) {
            meshPtr->updateBuffers(getVertices(player), localIndices);
        }

        Engine.updateCameraDefault3D();
        Engine.drawFrame();
        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}