// Multi_verse DeliveryService 
// Gavin Zimmerman

#ifndef CAMERA_H
#define CAMERA_H


// DEPENDENCIES
#include "game.h"
#include "signal.h"

#include <stdarg.h>
#include <stddef.h>


// CLASSES


// MEMBERS
void cameraInit(GLFWwindow* window);
void cameraStep(GLFWwindow* window, double t, double step);
void cameraCursorInput(GLFWwindow* window, double xpos, double ypos);

#endif