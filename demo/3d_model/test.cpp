/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#include "../../include/HammerEngine/HammerEngine.h"
#include <memory>
#include <glm/glm.hpp>


int main() {
    HammerEngine Engine;

    Engine.enableValidationLayers = true;
    Engine.WindowWidth = 1200;
    Engine.WindowHeight = 900;
    Engine.MaxTextures = 1000;
    Engine.mouseLock = 1; 
    Engine.windowName = "Hammer Engine - Model";
    Engine.renderDistance = 1000.0f;
    Engine.cameraSpeed = 1.0f;

    Engine.cameraPosition = glm::vec3(5.0f, 5.0f, 15.0f);
    Engine.cameraFront = glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f));

    Engine.initWindow();
    Engine.initVulkan();

    std::string vPath = "shaders/vert.spv";
    std::string fPath = "shaders/frag.spv";
    auto mainPipeline = std::make_unique<HammerPipeline>(Engine, vPath, fPath, 1, true);

    HammerModel model("model/teapot.obj");

    auto dirtTexture  = std::make_unique<HammerTexture>(Engine, "textures/texture.png", HammerTextureFilter::Nearest);

    auto myMesh = std::make_unique<HammerMesh>(Engine, mainPipeline.get(), dirtTexture.get(), model.vertexData, model.indexData);
    
    Engine.meshs.push_back(std::move(myMesh));

    Engine.drawPassStart();
    while (!glfwWindowShouldClose(Engine.window)) {
        Engine.updateFrameTimeStart();

        Engine.updateCameraDefault3D();
        
        Engine.drawFrame(); 
        
        Engine.updateFrameTimeEnd();
    }
    Engine.drawPassEnd();

    dirtTexture.reset();

    return EXIT_SUCCESS;
}