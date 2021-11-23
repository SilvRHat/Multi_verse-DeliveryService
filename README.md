

# Documentation Notes
## 11/22/2021

I had not initially expected the amount of setup this project would require. So I thought I'd expand on that here.

### GLFW Library
In previous projects, we have used glut as a utility library for input mapping/etc. However, in implementing this game - I had wanted more control for designing a game loop in addition to additional input controls. This shifted focus to using glfw or sdk - where glfw seemed most ideal, as it is maintained well and has a focused usage.
This required the rewriting low-level structure of my program.

### Camera Controller
My Camera controller went through significant changes - improving readability and OpenGL Dependencies.
First, I've defined a 'cameraStep' function to handle the updates between frames of all camera contexts. This will calculate the CFrame using two DOF (azimuth and inclination angles)

### OpenGL 4.0 Style
Starting this project and building a library of simple objects to start world-building, I set out to use the OpenGL 4.0 style for various reasons. 
- Most importantly, I wanted the program to be very efficient in CPU and GPU usage so that I could be ambitious with the detail in my scenes and shaders. Thus, I wanted my objects to use VAOs/ VBOs so that they may be cached in VRAM by OpenGL.
- I had also sourced a linear algebra library, which satisfied one of the largest pre-requisites to the OpenGL syntax.

### Build Scene Data Once & Render Later
Instead of rendering the scene by calling object functions (cube, sphere, etc.) - what I did here was build an array of objects that contains any needed data to be rendered later on a seperate function call. This allows a few things to happen:
- Object Culling
- Easy map/scene rendering multiple times in various spots
- Simple Animation via Signal Patterns
- Possible loading/unloading if levels become very large