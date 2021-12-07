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
To run program, run: <code>./Mv-DS</code>


## Controls
    Control                     Description
-   MouseWheel                  Zoom In/ Out
-   Right Mouse Button          Move Camera (DEV MODE)
-   Middle Mouse Button         Move Camera (DEV MODE)
-   Scroll Wheel                Zoom In/ Out



## Project Review
My project review demonstrates the technology/ framework of my project. This includes:
- Mouse-Based Camera Controller 
- Instances/ objects including (line, plane, circle, cube, sphere, cone, frustum)
- Drawing pipeline (scene to render, objects to cull, animations to play, etc.)
- Simple shader
- Simple scene


In the last week I plan on adding the majority of my content, including:
- Scene 'jumping' (Portal logic)
- Character
- 4 more scenes (all fully planned out with concept art)
    - Chracter home universe (half-built room)
    - Expansion of my hw4; torri gate/ waterfall scene
    - Igloo village
    - Technology city
- New Shaders


PROGRESS STATEMENT: I'm behind in my project's visual content - I had attempted a lot of setup converting my code structure to work with glfw and OpenGL 3.0+, without a good knowledge/estimate of the time it would take. However, I feel that this setup has greatly benefited my knowledge of graphics concepts/tools; and enabled powerful development for this game. 


Some components I'm proud of:
- Camera controller; Is mouse based allowing easy viewing & quick development. Built with custom 4D matrix operations and contains 2 positions (viewing position, and a pivot position). A public function allows other code to retrieve both positions, which can be made to set the character position.
- Signal Pattern; I've involved signals in various pieces of my code which allow the dynamic binding/unbinding of functions (similar to callbacks). Each scene/verse invokes a signal when rendered allowing animations per object to run. This can also be used to supliment billboards, where the rotation is set the same as the camera.
- Rendering pipeline: In my render.c file, I enable rendering a scene by an array of object data. This will allow me to find all scenes I need to render, offsets to make, and the rending of many dynamic scenes for portal logic. In this process I can cull objects.
- VAO/VBO based objects: I've drawn many of the simple objects in VBOs often using triangle-strips to reduce vertices. Certain objects (such as spheres) allow specification on the number of vertices; which I've standarized allowing for VBO reuse in addition to custom generation of objects.


Additional:
This project has been the most fun coding I've done in a while - and I've enjoyed putting a lot of freetime into it. I plan on using this as a launching piece in my career, publishing it and showcasing it in my portfolio. I'm happy to work on this past the semester and deadline, but I've found this has made me patient in progress on deliverables. In your feedback, please feel free to let me know a good way to combat this and adjut my focus to be successful in the final deliverable - as I know I may have shifted off track.

OpenGL concepts used:
- OpenGL Geometry instancing [For performance]
- Geometry shaders [For visual effects]



### OpenGL 4.0
My project uses the OpenGL 3+ Syntax/ Core Profile - in which many of the OpenGL 2.0 concepts we've learned in class have been deprecated. I though to provide some insight and reasoning on why I pursued this path, because it did take a lot of time for the setup of basic elements.
 - Did I really need OpenGL 4.0 for my project? No. OpenGL 2.0/ Fixed pipeline is incredibly versatile on its own and I have no doubt that I would have been able to accomplish a lot without OpenGL 3+.
 - So why? Well, I really I wanted to get into the low level pieces and get a deep understanding of many graphics concepts beyond whats taught in class. And I did! I learned how I should use VAOs/ VBOS, how they differ, the internal mechanics of shaders, how to use transformation matrices for everything, and a lot more.
 - What I gained? Pipeline Control (which I greatly plan on using). Building a customized pipeline - I was able to utilize VAO/VBOs to save computation cost. Current shader concepts including geometrey shaders (which I plan on using). And a STRONG/ versatile codebase, objects are often built once and rendered/drawn when needed on a per-part basis.