/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HammerEngine/HammerEngine.h"
#include <iostream>
#include <vulkan/vulkan_core.h>

using namespace std;

void HammerEngine::runTest() {
    initWindow();
    initVulkan();
    mainLoopTest();
    cleanup();
}

void HammerEngine::initWindow() {

    if(mouseLock == 1){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, HammerEngine::framebufferResizeCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwMakeContextCurrent(window);
    } else if(mouseLock == 0){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, HammerEngine::framebufferResizeCallback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwMakeContextCurrent(window);
    }
}

void HammerEngine::drawPassEnd(){
    vkDeviceWaitIdle(device);
}

void HammerEngine::mainLoopTest() {
    drawPassStart();

    while (!glfwWindowShouldClose(window)) {
        updateFrameTimeStart();

        updateCameraDefault3D();

        drawFrame();

        updateFrameTimeEnd();
    }

    drawPassEnd();
}

void HammerEngine::updateFrameTimeEnd(){
    auto end = std::chrono::high_resolution_clock::now();
    auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    totalFrameTime += frameDuration;

    if (currentTime - previousTime >= 1.0) {
        double averageFrameTime = totalFrameTime.count() / static_cast<double>(frameCount);

        std::cout << "FPS: " << frameCount 
                  << " | Avg Frame Time: " << averageFrameTime << " ms\n";

        frameCount = 0;
        previousTime = currentTime;
        totalFrameTime = std::chrono::milliseconds(0);
    }
}

void HammerEngine::drawPassStart(){
    previousTime = glfwGetTime();
    frameCount = 0;

    std::chrono::milliseconds totalFrameTime(0);

    //vertexBufferSize = sizeof(Vertex) * vertices.size();
    //indexBufferSize = sizeof(uint32_t) * indices.size();
}

void HammerEngine::updateFrameTimeStart(){
    start = std::chrono::high_resolution_clock::now();

    currentTime = glfwGetTime();
    frameCount++;
    glfwPollEvents();
}

void HammerEngine::setMaxVertciesIndicesSize(VkDeviceSize maxsize){
    stagingBufferSize = maxsize;
    stagingBuffer2Size = maxsize;
}