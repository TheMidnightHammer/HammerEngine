# Hammer Engine
HammerEngine is an open source Engine using Vulkan as backend.
It supports simple collision detection, there is an example in the test folder.

[Showoff.webm](https://github.com/user-attachments/assets/d3d87ad2-bde6-4431-944c-7280afff7d88)

# The goal of Hammer Engine
The goal of the Hammer Engine is of combining ease of use and speed of Vulkan to make 3d and 2d games as fast and fun as possible.

# The performance of Hammer Engine

The number 1 goal of HammerEngine is performance on low-end machines. It out performance most engines while having support for many machines.

# What does Hammer Engine support?

Vulkan Native: Built from the ground up for modern hardware efficiency.

Collision System: Includes a functional, simple collision detection system.

Developer Friendly: Designed for rapid prototyping without sacrificing performance.

# Run demos

## Linux

1. To build Hammer Engine you will need to install vulkan, vulkan validation layers, spirv tools and stb.

`sudo apt-get install libvulkan-dev vulkan-tools vulkan-validationlayers spirv-tools libstb-dev`

2. Test your vulkan install

`vkcube`

3. You will need to clone the latest version of Hammer Engine

`git clone https://github.com/MidnightHammer-code/HammerEngine.git`

4. Go in the HammerEngine/tests/3d_world folder or any other test

`cd HammerEngine/tests/3d_world/`

5. Make a folder called build and go in the build folder

`mkdir build && cd build`

6. Run Cmake and make to prepare the build and build it using all available cores

`cmake .. && make -j(nproc)`

7. Move the executable to the main demo folder and go to the main demo folder

`mv voxel_Engine .. && cd ..`

8. Run the test

`./voxel_Engine`

## Windows & macOS

We are currently looking for contributors to help establish build instructions for Windows and macOS. If you have successfully compiled Hammer Engine on these platforms, please consider submitting a Pull Request or opening an issue with your steps!

# Want to contribute?
We are looking for contributors! Since we are in the alpha/beta stage, now is the best time to help shape the engine's architecture. We are especially looking for help with:

1. Optimizing the Engine even more for low-end machines.
2. Extended physics/collision features.
3. Separate the functions from HammerEngine.cpp into separate files.
4. Add a class for creating pipelines and managing them.
5. Add a class for creating draw calls (not Render pass)
6. Add documentation, to help people learn Hammer Engine.

# How to use?

When the first non-alpha-beta version will release, there will be a guide to help you learn HammerEngine.
