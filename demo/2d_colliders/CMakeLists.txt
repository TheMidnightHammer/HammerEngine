cmake_minimum_required(VERSION 3.10)

project(voxel_Engine VERSION 2.0)

set(SOURCE_FILES
    test.cpp
    ../../scr/HammerEngine.cpp
    ../../scr/HammerUtility.cpp
    ../../scr/HammerCamera.cpp
    ../../scr/HammerRect.cpp
    ../../scr/HammerBuffers.cpp
    ../../scr/HammerPipeline.cpp
)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)

find_package(glfw3 REQUIRED)

include_directories(
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    vulkan
    glfw
    stb
)
