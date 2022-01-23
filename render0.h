// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#ifndef RENDER_H
#define RENDER_H

// DEPENDENCIES
#include "graphics.h"
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


struct InstanceRenderData_s {
    void* Instance;
    mat4x4 ViewMat;
};
typedef struct InstanceRenderData_s InstanceRenderData;


struct RenderStackData_s {
    InstanceRenderData Lights[MAX_VERSE_LIGHTS];
    InstanceRenderData Parts[MAX_VERSE_PARTS];
    
    int NumLights, NumParts;
};
typedef struct RenderStackData_s RenderStackData;



// MEMBERS
void renderInit(GLFWwindow* window);
void renderExit(GLFWwindow* window);
void renderStep(GLFWwindow* window, double t, double step);

#endif