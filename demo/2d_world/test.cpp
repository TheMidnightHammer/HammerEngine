#include "../../include/HammerEngine/HammerEngine.h"
#include <vector>
#include <glm/glm.hpp>

// === Entry Point ===
int main() {
    HammerEngine Engine;

    vertices = {
        // Front (tile 0,0)
        {{-0.5f,-0.5f, 0.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},
    };

    indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
    };

    Engine.vertShaderPath = "shaders/vert.spv";
    Engine.fragShaderPath = "shaders/frag.spv";
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.texturePath = "textures/texture.png";
    Engine.cameraMode2dOr3d = 0;
    Engine.cameraSpeed = 0.1;
    Engine.renderTriangleMod = 0;
    Engine.triangleRender2SideMode = false;
    Engine.renderDistance = 16.0f;

    Engine.initWindow();
    Engine.initVulkan();

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        Engine.updateCameraDefault2D();
        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}
