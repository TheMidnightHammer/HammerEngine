# Hammer
HammerEngine is an open source Engine using Vulkan as backend.
It supports simple collision detection, there is an example in the demo folder.

[Showoff.webm](https://github.com/user-attachments/assets/d3d87ad2-bde6-4431-944c-7280afff7d88)

# The goal of Hammer Engine
The goal of the Hammer Engine is combining ease of use and the speed of Vulkan to make 2d/3d games as fast and fun as possible.

# The performance of Hammer

The #1 goal of HammerEngine is it's performance on low-end machines. It out-performs many other engines while remaining compatable with as many machines as possible.

# Cross-platform

Hammer supports for all major linux distro, windows (comming soon) and chromebook.

# What does Hammer support?

Vulkan Native: Built from the ground up for modern hardware efficiency.

Collision System: Includes a functional, simple collision detection system.

Developer Friendly: Designed for rapid prototyping without sacrificing performance.

Easy to load 3d models: Uses tinyobjloader to load .obj files discards any vertex that is duplicated.

<img width="632" height="706" alt="image" src="https://github.com/user-attachments/assets/157a91fb-d69d-4deb-9de6-efdec45e3402" />
<img width="632" height="706" alt="Screenshot from 2026-03-18 09-32-32" src="https://github.com/user-attachments/assets/30bb86b6-7093-4a84-9413-d55ffb2b04e6" />
<img width="1228" height="966" alt="image" src="https://github.com/user-attachments/assets/7d6a529a-c3ba-4a30-881c-aeba3b27118b" />


# Docs

The official tutorial is at docs/

# Run demos

## Linux

1. Run the build.sh
`./build.sh`

2. The build will ask which demo you want to run you can put the path to any demo in the demo folder.

3. Run the demo and enjoy.

## Windows & macOS

We are currently looking for contributors to help establish build instructions for Windows and macOS. If you have successfully compiled Hammer on these platforms, please consider submitting a Pull Request or opening an issue with your steps! For windows please check this [branch](https://github.com/MidnightHammer-code/HammerEngine/tree/windows-support) for the latest detals for windows.

# Want to contribute?
We are looking for contributors! If you want to contribute to Hammer go to the issues tab pick an issue and create a fix for it. If you have an idea for a new feature you can create an issue with the label New feature or implement it and create a pull request. We are especially looking for help with:

1. Optimizing the Engine even more for low-end machines.
2. Extended physics/collision features.
3. Separating the functions from HammerEngine.cpp into separate files.
4. Add documentation, to help people learn Hammer Engine.

# How to use?

The Hammer Engine tutorial is being made right now and should be finished very soon. you can find it at https://github.com/MidnightHammer-code/HammerEngine/tree/main/docs.
