# OpenGL renderer

This is a small OpenGL renderer I wrote following the [LearnOpenGL website](https://learnopengl.com/), in C++. It implements the following features:
* A moving camera controlled through WASD and the scroll wheel.
* Multiple types of lights: directional lights, point lights, and spotlights.
* Basic Blinn-Phong shading.
* Model-loading using the ASSIMP library.
* Object outlining through stencil testing.
* Blending for transparency effects.

## Screenshots
![A side view of a ukulele and backpack model, made using this renderer](screenshots/model_side.png "A side view of a ukulele and backpack model, made using this renderer")
![A front view of a ukulele and backpack model, made using this renderer](screenshots/model_front.png "A side view of a ukulele and backpack model, made using this renderer")
![A view of stone blocks, with grass in front of some of their sides](screenshots/depth.png "A view of stone blocks, with grass in front of some of their sides")

I have since moved on to learning Vulkan, so this project will be on indefinite hold until then.
