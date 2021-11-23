// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

// PROJECT HEADER
#ifndef MULTIVERSE_DELIVERY_SERVICE_H
#define MULTIVERSE_DELIVERY_SERVICE_H


// DEPENDENCIES
// Graphics
#ifdef USEGLEW
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#endif


// GLOBALS
extern struct signal   
    KeyInput, MouseButtonInput, ScrollInput, CursorInput;


#endif