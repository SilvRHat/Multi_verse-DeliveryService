// Multi_verse DeliveryService
// Gavin Zimmerman

// Mutli_verse DeliveryService Globals
#ifndef MULTIVERSE_DELIVERY_SERVICE_H
#define MULTIVERSE_DELIVERY_SERVICE_H


// DEPENDENCIES
#include "signal.h"

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
extern SignalInstance  
    KeyInput, MouseButtonInput, ScrollInput, CursorInput;


// CONFIG
#define DEVMODE 1


#endif