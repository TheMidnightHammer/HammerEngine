# Copyright (c) 2026 MidnightHammer-code
# This source code is licensed under the GPL 3.0 license

cmake_minimum_required(VERSION 3.10)
project(voxel_Engine VERSION 2.0)

# 1. Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Source Files (Relative to the CMakeLists.txt location)
set(SOURCE_FILES
    test.cpp
    ../../src/HammerEngine.cpp
    ../../src/HammerUtility.cpp
    ../../src/HammerCamera.cpp
    ../../src/HammerRect.cpp
    ../../src/HammerBuffers.cpp
    ../../src/HammerPipeline.cpp
)

# 3. Find Dependencies
find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED) # Standard for most package managers
find_package(glfw3 REQUIRED)

# 4. Create Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 5. Link Libraries (The Modern Way)
# This automatically handles include_directories for you.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)

# 6. Windows-Specific Compiler Flags (MinGW/GCC)
if(WIN32 AND NOT MSVC)
    # Static link standard libs so the .exe runs without needing MSYS2 DLLs
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
    target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
    
    # Optional: Remove the console window in the background for a GUI app
    # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# 7. Linux-Specific Flags
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl)
endif()