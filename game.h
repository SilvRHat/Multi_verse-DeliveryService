// Multi_verse DeliveryService
// Gavin Zimmerman

// Mutli_verse DeliveryService
#ifndef MULTIVERSE_DELIVERY_SERVICE_H
#define MULTIVERSE_DELIVERY_SERVICE_H

// DEPENDENCIES
#include "signal.h"
#define DEVMODE 1 // For .vscode

// Graphics
#ifdef USEGLEW
#include <GL/glew.h>
#endif

//#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
//#include <OpenGL/glu.h>
#endif



// GLOBALS
int DEFAULT_SHADER;

// Signals
extern SignalInstance  
    KeyInput, MouseButtonInput, ScrollInput, CursorInput;

#include <stdio.h>
void Error(const char* format , ...);
void ErrCheck(const char* where);


#endif