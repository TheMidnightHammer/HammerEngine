#include "../../include/HammerEngine/HammerEngine.h"
#include "../../include/HammerEngine/HammerRect.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include <glm/glm.hpp>

// === Entry Point ===
int main() {
    HammerEngine Engine;

    HammerRectSquareF square{0,0, 1,1};
    HammerRectSquareF player{0,2, 1,1};

    vertices = {
        // Front (tile 0,0)
        {{player.x + -0.5f, player.y + -0.5f, 0.0f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}}, //player
        {{player.x +  0.5f, player.y + -0.5f, 0.0f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{player.x +  0.5f, player.y +  0.5f, 0.0f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{player.x + -0.5f, player.y +  0.5f, 0.0f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},

        {{-0.5f,-0.5f, 0.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}}, //cube
        {{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},
    };

    indices = {
        // Player quad
        0, 1, 2,
        2, 3, 0,

        // Cube front face
        4, 5, 6,
        6, 7, 4
    };

    Engine.setMaxVertciesIndicesSize(sizeof(Vertex) * vertices.size());
    Engine.vertShaderPath = "shaders/vert.spv";
    Engine.fragShaderPath = "shaders/frag.spv";
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1000;
    Engine.WindowHeight = 1000;
    Engine.texturePath = "textures/texture.png";
    Engine.cameraPosition = glm::vec3(0,0,2);
    Engine.cameraMode2dOr3d = 0;
    Engine.cameraSpeed = 0.1;
    Engine.renderTriangleMod = 0;
    Engine.triangleRender2SideMode = true;
    Engine.renderDistance = 16.0f;

    Engine.initWindow();
    Engine.initVulkan();

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        
        if (glfwGetKey(Engine.window, GLFW_KEY_I) == GLFW_PRESS){
            player.y+=0.1;
            Engine.updateVertexIndexBuffers();
        }
        if (glfwGetKey(Engine.window, GLFW_KEY_K) == GLFW_PRESS){
            player.y-=0.1;
        }
        if (glfwGetKey(Engine.window, GLFW_KEY_J) == GLFW_PRESS){
            player.x-=0.1;
        }
        if (glfwGetKey(Engine.window, GLFW_KEY_L) == GLFW_PRESS){
            player.x+=0.1;
        }

        vertices = {
            // Front (tile 0,0)
            {{player.x + -0.5f, player.y + -0.5f, -1.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}}, //player
            {{player.x +  0.5f, player.y + -0.5f, -1.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
            {{player.x +  0.5f, player.y +  0.5f, -1.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
            {{player.x + -0.5f, player.y +  0.5f, -1.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},

            {{-0.5f,-0.5f, -1.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}}, //cube
            {{ 0.5f,-0.5f, -1.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
            {{ 0.5f, 0.5f, -1.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
            {{-0.5f, 0.5f, -1.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}},
        };

        // auto start = std::chrono::high_resolution_clock::now();

        Engine.updateVertexIndexBuffers();

        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end - start;

        // std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " milliseconds\n";

        Engine.updateCameraDefault3D();

        Engine.drawFrame();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}
