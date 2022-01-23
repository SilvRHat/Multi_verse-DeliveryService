# Multi_verse DeliveryService
Developed by Gavin Zimmerman

This is my submission for a final project in a Computer Graphics course taught at Univeristy of Colorado, Boulder by Willem A. (Vlakkies) Schreuder.

My motivation for developing Mv-DS was to create a unique game based soley around graphics and various styles. My main inspiration for developing worlds of differing styles was from Spider-Man: Into the Spider-Verse. So I set out to develop a mail delivery robot that travels around the multi-verse delivering messages from worlds of every style.



## Dependencies
 - OpenGL 3.0+
 - GLFW 3.0+
 - MacOS X / Linux / Windows  Operating System
    - Only tested on MacOS X/ Linux thus far

## Running
Running the program
To compile, run: <code>make</code>
To run program, run: <code>./final</code>


## Controls
    Control                         Description
-   Scroll Wheel                    Zoom In/ Out
-   Right Mouse Button              Rotate Camera
-   RIGHT Mouse Button + SHIFT      Move Camera
-   Middle Mouse Button             Rotate Camera
-   Middle Mouse Button + SHIFT     Move Camera
-   WASD                            Move Camera


## Status
Developing this game has been beyond fun- but its nowhere near done and I can't wait to get it there over winter break. And getting to the project submission deadline I also think my exisiting codebase makes for a great submission. So enjoy reading about the crazy stuff I made below!


## Check these parts out:
- Portal implementation: This was a main feature I had described in my proposal and is one of the most involved pieces in my code.
    - Portals are tracked by a 'JumpInstance' which holds two homogenous coordinates which I call CFrames (coordinate frames). Defined in [objects.h 84:0].
    - When JumpSetCFrames() is called, the two CFrames are set along with pre-computed translation CFrames (_ToV0, _ToV1). [objects.c 748:0]. These matrices allow the origin of an external scene (with its own coordinate system) to be treated as an offset. 
    - Rendering with portals is a two step process. The first step is gathering what scenes should be rendered and at what offset. [render.c 123:0 to 221:0]. We actually need these in order of depth to properly render, so we step through each scene and populate a 'RenderStack' until it is full.
        - For performance, I implemented a portal-culling technique that will only render external scenes if they are visible through all the portals taken to get there. To test this, I keep track of a bounding box in NDC coordinates. [render.c 175:0]. I've provided a video of this specific implementation in the /doc folder.
    - The second step of rendering portals involves using the stencil buffer. First, the farthest scene needs to be rendered so that the closer scenes don't populate it with objects. 
        - Because of this behaviour, we need to use GL_INCR in the glStencilOp to ensure that the individual pixel is visible through all portals.
        - After setting the stencil buffer and rendering the external scene (with clipping), a z-depth-only pass on the individual portal shields closer scenes from writing into that space.
    - Sadly, you cannot go through portals yet due to difficulty in how the camera controller is setup (using angles which requires euler angle calculations from homogenous coordinates). The method of crossing through a portal with a third person camera also comes with many occlusion problems - not to say I don't already have a method of combatting them.

- Signal-based programming: This adheres to the event-programming model discussed in lecture 2- and allows for easy animation. Every time the scene is rendered, it fires a signal that calls back any connected functions. I use this to animate my scene.

- Various objects drawn with triangle primitives: I've drawn many of the simple objects in VBOs often using triangle-strips to reduce vertices. Certain objects (such as spheres) allow specification on the number of vertices; which I've standarized allowing for VBO reuse in addition to custom generation of objects. This allows for the design of complex scenes from simple building blocks. I've also built a function for building arcs out of any part.




### OpenGL 4.0
My project uses the OpenGL 3+ Syntax/ Core Profile - in which many of the OpenGL 2.0 concepts we've learned in class have been deprecated. I though to provide some insight and reasoning on why I pursued this path, because it did take a lot of time for the setup of basic elements.
 - Did I really need OpenGL 4.0 for my project? No. OpenGL 2.0/ Fixed pipeline is incredibly versatile on its own and I have no doubt that I would have been able to accomplish a lot without OpenGL 3+.
 - So why? Well, I really I wanted to get into the low level pieces and get a deep understanding of many graphics concepts beyond whats taught in class. And I did! I learned how I should use VAOs/ VBOS, how they differ, the internal mechanics of shaders, how to use transformation matrices for everything, and a lot more.
 - [UPDATED] What I gained? I especially plan on utilizing the instancing versions for drawing objects so as to reduce CPU/GPU communication bottlenecks. And I plan on using geometry shaders for various effects on character and on environment such as rendering lines to showcase wind. Many of my objects are built with the same shader and VBO, thus instancing is very suitable for larger development.
