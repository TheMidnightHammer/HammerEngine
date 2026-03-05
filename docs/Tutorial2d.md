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
In Hammer Engine you can change a few things of the window. These things include: window size (w and h) but also the title of the screen.

```
Engine.WindowWidth = 1000;
Engine.WindowHeight = 1000;
Engine.windowName = "Hammer Engine";
```

### 2.3 Camera position
You can set the starting position of the camera.
`Engine.cameraPosition = glm::vec3(0, 0, 2);`

### 2.4 The texture atlas path
You can give Hammer Engine the path to a texture, but only 1 in the future Hammer Engine will have a way to have many textures but for now only 1.
`Engine.texturePath = "textures/texture.png";`

### 2.5 The mouse
If you are building a app that you need to have the controle over the mouse cursor you can call change the cursor state.
If mouse lock it set to 0 the mouse will be free to move, if it 1 the mouse can not longer move.
`Engine.mouseLock = 0;`
