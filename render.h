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
#include "multiverse.h"


// Classes
#define MAX_RENDER_JUMP_DEPTH 32
#define MAX_RENDER_VERSES 128
struct JumpRenderData_s {
    // Book Keeping
    int Depth;
    void *Travel[MAX_RENDER_JUMP_DEPTH];
    double Radius;
    // Translation
    VerseInstance* VerseTo;
    mat4x4 OffsetFrom, OffsetTo;
    // Validation 
    vec4 Bounds;
    float Z_Bound;
};
typedef struct JumpRenderData_s JumpRenderData;


// MEMBERS
void renderInit(GLFWwindow* window);
void renderExit(GLFWwindow* window);
void renderStep(GLFWwindow* window, double t, double step);
void DrawInstance(PartInstance* part);


#endif