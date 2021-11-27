# Multi_verse DeliveryService
Developed by Gavin Zimmerman

This is my submission for a final project in a Computer Graphics course taught at Univeristy of Colorado, Boulder by Willem A. (Vlakkies) Schreuder.

My motivation for developing Mv-DS was to create a unique game based soley around graphics and various styles. My main inspiration for developing worlds of differing styles was from Spider-Man: Into the Spider-Verse. So I set out to develop a mail delivery robot that travels around the multi-verse delivering messages from worlds of every style.


## Developer Status
Estimated Progress [25%]
Currently, I am developing the framework of this project. Setup still required includes:
 - Arbitrarily rendering scenes
 - Jump logic
 - OpenGL 4.0 Syntax
 - Shaders
 - Character Controller


## Dependencies
 - OpenGL 2.0+
 - GLFW 3.0+
 - MacOS X / Linux  Operating System
    - Only tested on MacOS X thus far

## Running
Running the program
To compile, run: <code>make</code>
To run program, run: <code>./Mv-DS</code>


## Controls
    Control                     Description
-   MouseWheel                  Zoom In/ Out
-   Right Mouse Button          Move Camera (DEV MODE)
-   Middle Mouse Button         Move Camera (DEV MODE)


## Documentation
The following documents some of my design considerations, implementation, and why this project deserves an A grade.

## Game Lore
Coding what you like is fun - seriously! So in addition to code documentation I wanted to give personality, life, history to the project by introducing a backstory/ narrative/ lore section to the header files of all level source code.


### Summary

### OpenGL 4.0
My project uses the OpenGL 3+ Syntax/ Core Profile - in which many of the OpenGL 2.0 concepts we've learned in class have been deprecated. I though to provide some insight and reasoning on why I pursued this path.
 - Did I really need OpenGL 4.0 for my project? No. Absolutely Not. OpenGL 2.0 is incredibly versatile on its own and I have no doubt that I would have been able to accomplish a lot without OpenGL 3+. This implementation took at least a full 2 days of setup.
 - So why? Well, I love programming and especially graphics - and I wanted to get into the low level pieces and get a deep understanding of many graphics concepts beyond whats taught in class. And I did! I learned how I should use VAOs/ VBOS, how they differ, the internal mechanics of shaders, how to use transformation matrices for everything, and more.
 - What I gained? Performance Control. Building a customized pipeline - I was able to utilize VAO/VBOs to save computation cost. Current shader concepts including geometrey/ dessolation shaders. A possible strong background for the Advanced Computer Graphics class.


### Time Spent
Sunday 11/21/2021
Driving Home in Car: Designed core functionality structure on glfw

Monday 11/22/2021
Spent day building camera controller

Tuesday 11/23/2021
Spent day defining core render structure - building render controller and instance standard for rendering part instances in a verse instance designated anywhere on map.

Wednesday 11/24/2021
Spent day working out OpenGL compatability - rendering a scene with OpenGL 4.0 with emission shader.

Thursday 11/25/2021
Build out primitatives - and drawing procedures for each


### Process
32 (working) hours into this project I realize I still am working on setup and code not directly visible in a final product - in fact I actually have no code for objects/visuals displayed in my deliverable (minus an emission shader). And I am very happy in my progress - in which I've built a robust framework for large-scale development with full compataibility to new OpenGL and GLSL developments. My first milestone was creating a camera controller - that mirrored Blender's camera controller for quick 3D modeling. I sourced logic from previous homeworks to achieve this, but had completely redone the source code using a linear algebra library.
Next, I began to work on building a standardized way for creating world heirachrys full of parts - so that through portal jumps any arbitrary world could be rendered (instead of a fixed relation between every game part).
From there, I expanded on developing my program to work with OpenGL 4.0, so that I would have full ability to OpenGL 4.0 GLSL syntax, efficient VAO/VBO logic, and an up-to-date/ background of OpenGL 4.0. This took a day on its own entirely, as I spent most of it trying to render the basic scene I had tested my camera controller on.


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