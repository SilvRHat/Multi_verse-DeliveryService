// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#ifndef RENDER_H
#define RENDER_H

// DEPENDENCIES
#include "game.h"
#include "linmath.h"
#include "objects.h"

// MEMBERS
void renderInit(GLFWwindow* window);
void renderStep(GLFWwindow* window, double t, double step);


#endif