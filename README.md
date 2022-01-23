# Multi_verse:RE(Rendering Engine)
Gavin Zimmerman


## Multi_verse:RE //
 Mv:RE is an engine for rendering multi-styled 3D scenes for games, animations, and 3D builds. The engine provides two core pieces: a set of structures for building such projects and a rendering pipeline for drawing them. 


## Multi_verse Rendering //
 This method of 3D creating is inspired from Spider-Man: Into the Spider-Verse, which embraces the collaboration of multiple styles together. Mv:RE also embraces this collaboration. The following outlines core technologies in graphics and expands on their use in Mv:RE for rendering multi-styled scenes.


### Forward-Rendering //
 Forward rendering is a common technique for rendering scenes with multiple/arbitrary light sources, by rendering the objects for each light that affecting it and performing an additive blend on the resulting color. This technique can easily become expensive in heavily-lighted scenes as opposed to other techniques like deferred rendering. The tradeoff here is freedom in shading and is more typical in an animation/mobile game context.

### Shaders //
 Shaders are graphics programs, used for lighting, procedural textures, and many other special purposes. They require state-change which is expensive in the context of graphics, and are often standardized/ grouped by custom material in games for run-time efficiency. 

### Lights //
 In the context of forward-rendering, lights are essentially a rendering pass adding new information into the color buffer through additive blending. Note that this does not translate to one draw call, it simply means any affected parts get rendered under it.

### In the context of Mv:RE //
 - Light Layering - Light instances are provided a property that determines its priority in rendering
 - Light Blending - Lights may customize the blending functions / constants
 - FBO/Light Chaining - Post-processing often involves an initial render to a texture which is read on a secondary pass. Chaining the results of previous passes allows shading processes to be split between multiple passes. For unified calculations like lighting followed by seperate calculations like procedural texturing
 - Noding - Shaders are used for similar tasks, but often differ by parts/materials. These parts are allowed multiple shaders for those similar tasks allowing 2 parts to be shaded differently by the same light
 - Light OverShading - Lights can also opt to shade every part using the same fragment shader (overwriting the shader stages provided by the part)
 - Light Groups - Specifies which parts get affected by which lights; each part by default registers to a global light group to render without lights, but can unregister for unique effects

### Art considerations //
 While it can be easy to combine art styles, it can be very difficult to unify them. Think of your project as a team of differening visuals - you want these visuals to work together rather than clash/ fight. 

 Thus when combining art styles establish similarities which will unify them. Having cohesion on some element will enforce the feeling of a shared source in your creations.

 Similarly, you want to establish differences which make them unique. Here you want to contrast some element between the two styles which compliment each other. Be careful in establishing this contrast, since not all differences compliment each other the same.

 Some elements to think of include: color scheme, shapes/ objects, projection mode, lighting, animation, frame-rate, etc.


## Dependencies //
 - OpenGL 4.1+
 - glfw3
 - MacOS X / Linux
 - gcc


## Files //
 - README
    - Foreword on Mv:RE
 - MvRE.h
    - Multi_verse:RE Header / Sources dependencies
 - graphics.h
    - Graphics dependencies
 - linmath.h
    - Linear Algebra math library
 - signal.h / signal.c
    - Signal instance for dynamic event-programming
 - utils.h / utils.c
    - Additional utility functions including sorting
 - objects.h / objects.c
    - Multi_verse:RE Instances and packaged rendering objects
 - render.h / render.c
    - Multi_verse:RE Rendering Pipeline


## License //
 This engine is Open Source and uses a GPL License, similar to Blender. 