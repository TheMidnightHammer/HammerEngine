Welcome to the HammerEngine documentation. This tutorial will walk you through the evolution of a HammerEngine project, starting with basic 2D rendering and moving meshs around.
This tutorial asumes that you have a midium undersanding of C++.

Now lets get started.

## 1. Creating a Hammer Engine instance.

The foundation of any HammerEngine project involves configuring the engine settings, initializing Vulkan, and setting up a basic camera.

First we need to a instance of Hammer:
`HammerEngine engine;`

Now we have a instance of the engine called "engine".

## 2. Configuring the Engine.
### 2.1 Validation layers
Validation layers are there to give you more detaled errors than the one Vulkan gives you. For developpement we will turn them on.
`Engine.enableValidationLayers = true;`
You can always turn them off by just changine the true for a false.
### 2.2 Window
