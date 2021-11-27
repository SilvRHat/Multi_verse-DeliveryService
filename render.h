// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#ifndef RENDER_H
#define RENDER_H

// DEPENDENCIES
#include "game.h"
#include "linmath.h"
#include "objects.h"
#include "camera.h"

#include <string.h>

// VERSES
#include "home.h"


// MEMBERS
void renderInit(GLFWwindow* window);
void renderExit(GLFWwindow* window);
void renderStep(GLFWwindow* window, double t, double step);
void DrawInstance(PartInstance* part);

#endif