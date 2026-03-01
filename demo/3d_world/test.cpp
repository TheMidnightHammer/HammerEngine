#include "../../include/HammerEngine/HammerEngine.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

// === Constants ===
constexpr int GRID_X = 100; // from -50 to 49
constexpr int GRID_Y = 4;   // from -4 to -1
constexpr int GRID_Z = 100; // from -50 to 49

constexpr int OFFSET_X = 50;
constexpr int OFFSET_Y = 4;
constexpr int OFFSET_Z = 50;

// === Globals ===
std::vector<glm::vec3> cubePositions;
bool cubeExists[GRID_X][GRID_Y][GRID_Z] = {};

float thing = 0.01;

// Each face has 4 vertices:
static const Vertex faceVertices[6][4] = {
    // Top face
    {
        {{-0.5f, 0.5f,-0.5f},{1.0f,0.0f,0.0f},{0.1250f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.0f,1.0f,0.0f},{0.1875f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.1875f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.1250f,0.0000f}}
    },
    // Bottom face
    {
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.1250f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.5f,0.5f},{0.1250f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.0f},{0.0625f,0.0000f}}
    },
    // Right face
    {
        {{ 0.5f,-0.5f,-0.5f},{1.0f,0.5f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,1.0f,0.0f},{0.0625f,0.0000f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,1.0f,0.5f},{0.0000f,0.0000f}},
        {{ 0.5f,-0.5f, 0.5f},{0.5f,0.0f,1.0f},{0.0000f,0.0625f}}
    },
    // Left face
    {
        {{-0.5f,-0.5f,-0.5f},{0.5f,0.0f,0.5f},{0.0625f,0.0625f}},
        {{-0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{0.0f,0.5f,0.5f},{0.0000f,0.0000f}},
        {{-0.5f,-0.5f, 0.5f},{0.0f,0.0f,0.5f},{0.0000f,0.0625f}}
    },
    // Front face
    {
        {{-0.5f,-0.5f, 0.5f},{1.0f,0.0f,0.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f,0.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f,1.0f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f, 0.5f},{1.0f,1.0f,0.0f},{0.0000f,0.0000f}}
    },
    // Back face
    {
        {{-0.5f,-0.5f,-0.5f},{1.0f,0.0f,1.0f},{0.0000f,0.0625f}},
        {{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f,1.0f},{0.0625f,0.0625f}},
        {{ 0.5f, 0.5f,-0.5f},{0.5f,0.5f,0.5f},{0.0625f,0.0000f}},
        {{-0.5f, 0.5f,-0.5f},{0.0f,0.0f,0.0f},{0.0000f,0.0000f}}
    }
};

// Indices per face (6 per face)
static const uint32_t faceIndices[6][6] = {
    {0, 3, 2, 2, 1, 0}, // Top
    {0, 1, 2, 2, 3, 0}, // Bottom
    {0, 1, 2, 2, 3, 0}, // Right
    {0, 3, 2, 2, 1, 0}, // Left
    {0, 1, 2, 2, 3, 0}, // Front
    {0, 3, 2, 2, 1, 0}  // Back
};

// === Build Dense Grid of Cubes ===
void buildWorld() {
    for (int y = -4; y <= -1; y++) {
        for (int z = -50; z < 50; z++) {
            for (int x = -50; x < 50; x++) {
                cubePositions.push_back(glm::vec3(x, y, z));
                cubeExists[x + OFFSET_X][y + OFFSET_Y][z + OFFSET_Z] = true;
            }
        }
    }
}

// === Fast Constant-Time Neighbor Check ===
bool isCovered(const glm::vec3& pos) {
    static const glm::ivec3 directions[6] = {
        {0, 1, 0}, {0, -1, 0},
        {1, 0, 0}, {-1, 0, 0},
        {0, 0, 1}, {0, 0, -1}
    };

    for (const auto& dir : directions) {
        glm::ivec3 neighbor = glm::ivec3(pos) + dir;

        int nx = neighbor.x + OFFSET_X;
        int ny = neighbor.y + OFFSET_Y;
        int nz = neighbor.z + OFFSET_Z;

        if (nx < 0 || nx >= GRID_X || ny < 0 || ny >= GRID_Y || nz < 0 || nz >= GRID_Z)
            return false;

        if (!cubeExists[nx][ny][nz])
            return false;
    }

    return true;
}

static const glm::ivec3 faceDirs[6] = {
    {0, 1, 0}, {0, -1, 0},  // top, bottom
    {1, 0, 0}, {-1, 0, 0},  // right, left
    {0, 0, 1}, {0, 0, -1}   // front, back
};

void buildMesh(const std::vector<glm::vec3>& positions) {
    vertices.clear();
    indices.clear();

    uint32_t offset = 0;

    for (const auto& pos : positions) {
        if (isCovered(pos)) continue; // skip fully covered cubes

        for (int face = 0; face < 6; ++face) {
            glm::ivec3 neighbor = glm::ivec3(pos) + faceDirs[face];
            int nx = neighbor.x + OFFSET_X;
            int ny = neighbor.y + OFFSET_Y;
            int nz = neighbor.z + OFFSET_Z;

            bool neighborExists = (nx >= 0 && nx < GRID_X && ny >= 0 && ny < GRID_Y && nz >= 0 && nz < GRID_Z && cubeExists[nx][ny][nz]);

            if (!neighborExists) {
                // Add vertices and indices for this face only, with proper offset
                for (int i = 0; i < 4; ++i) {
                    Vertex v = faceVertices[face][i];
                    v.pos += pos - glm::vec3(1,1,1);
                    vertices.push_back(v);
                }
                for (int i = 0; i < 6; ++i) {
                    indices.push_back(faceIndices[face][i] + offset);
                }
                offset += 4;
            }
        }
    }
}

// === Entry Point ===
int main() {
    HammerEngine Engine;

    buildWorld();
    buildMesh(cubePositions);

    Engine.setMaxVertciesIndicesSize(sizeof(Vertex) * vertices.size());
    Engine.vertShaderPath = "shaders/vert.spv";
    Engine.fragShaderPath = "shaders/frag.spv";
    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 900;
    Engine.WindowHeight = 900;
    Engine.texturePath = "textures/texture.png";
    Engine.cameraMode2dOr3d = 0;
    Engine.cameraSpeed = 0.1;
    Engine.renderTriangleMod = 0;
    Engine.triangleRender2SideMode = true;
    Engine.renderDistance = 1024.0f;

    Engine.initWindow();
    Engine.initVulkan();

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        Engine.updateCameraDefault3D();
        Engine.drawFrame();
        Engine.updateVertexIndexBuffers();

        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    return EXIT_SUCCESS;
}