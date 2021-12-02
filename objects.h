// Multi_verse DeliveryService
// Gavin Zimmerman

// Objects
#ifndef OBJECTS_H
#define OBJECTS_H


// DEPENDENCIES
#include "game.h"
#include "linmath.h"
#include "signal.h"


#ifdef DEVMODE
#include <stdio.h>
#endif
#include <string.h>
#include <stdlib.h>


// CONST
#define VERTEX_DATA_ROW 7
#define MAX_INSTANCES 512
#define MAX_JUMP_INSTANCES 32


// Use same VBO for a defined set of vertices
// Make new VBO if user specifies anything different
#define INST_XHI_SEGMTS    128
#define INST_HI_SEGMTS     64
#define INST_MED_SEGMTS    32
#define INST_LOW_SEGMTS    16
#define INST_XLO_SEGMTS    8

#define INST_XHI_RINGS  64
#define INST_HI_RINGS   32
#define INST_MED_RINGS  16
#define INST_LOW_RINGS  8
#define INST_XLO_RINGS  4



// SIMPLE DATATYPES
typedef GLubyte color3[3];
typedef GLubyte color4[4];


// GAME INSTANCES
struct PartInstance_s {
    // Book Keeping
    char* Name;
    char* ClassName;
    // Positional / Translational
    mat4x4 CFrame;
    int HomeVerse;
    vec3 Size;
    vec3 Position;
    vec3 Rotation;
    // Vertices Data
    unsigned int vao, vbo;
    unsigned int triangles, vertices, rings;
    void (*_drawFunc)();
    // UV Mapping Data
    // Texture
    // Color & Lighting
    color4 Color;
    double Transpareny;
    unsigned int Shader;
    // Collision Properties
};
typedef struct PartInstance_s PartInstance;



struct JumpInstance_s {
    // Book Keeping
    char *Name;
    char *ClassName;

    // Jumping
    VerseInstance *Verse0, *Verse1;
    mat4x4 CFrame0, CFrame1;
    double Radius;
};
typedef struct JumpInstance_s JumpInstance;



struct VerseInstance_s {
    // Properties
    int VerseID;
    int _Loaded;
    char* Name;
    mat4x4 ProjectMatrix;

    PartInstance* Children[MAX_INSTANCES];
    JumpInstance* Jumps[MAX_JUMP_INSTANCES];

    // Methods
        // @brief Function to construct/ init the verse instance
        // @param self Verse Instance called from
        // @param window GLFW window context
    void (*Build) (struct VerseInstance_s*, GLFWwindow*);
        // @brief Function to deconstruct/ cleanup the verse instance
        // @param self Verse Instance called from
        // @param window GLFW window context
    void (*Clean) (struct VerseInstance_s*, GLFWwindow*);

    // Events
    SignalInstance RenderStepped;
};
typedef struct VerseInstance_s VerseInstance;





// Functions
// Constructors
PartInstance* NewPartInstance();
void* DeletePartInstanceBuffers(PartInstance* p);
void* DestroyPartInstance(PartInstance* p);

// Simple Objects / Primitive Building Objects
PartInstance* line(int n, vec4 pts[]);
PartInstance* plane(double size);
PartInstance* circle(int sides, double radius);
PartInstance* cube(double size);
PartInstance* uvSphere(int segments, int rings, double radius);
PartInstance* cylinder(int sides, double radius, double depth);
PartInstance* cone(int sides, double rad1, double rad0, double depth);
//PartInstance* wedge(double size); // TODO
//PartInstance* wedgecorner(double size); // TODO

// Loadable Objects
// PartInstance* meshFromObj(int* name, char* objpath); // TODO
PartInstance* clonePart(PartInstance* p);

// Custom Objects
    // NONE


// Part Methods
void PartSetColor(PartInstance* p, color3 c);
void PartSetCFrame(PartInstance* p, mat4x4 cf);
void PartSetSize(PartInstance* p, vec3 s);
void PartSetPosition(PartInstance* p, vec3 pos);
void PartSetRotation(PartInstance* p, vec3 r);
void PartSetTranslation(PartInstance* p, vec3 size, vec3 position, vec3 rotation);
void PartSetMetaProperties(PartInstance* p, char* Name, char* ClassName, int HomeVerse);
void PartSetShader(PartInstance* p, unsigned int shader);




// Jump Functions
// Constructors / Destructors
JumpInstance* NewJump();
void *DestroyJump(JumpInstance* j);
void JumpConnectVerses(VerseInstance* v0, VerseInstance* v1);
void JumpSetCFrames(mat4x4 CF0, mat4x4 CF1);




// Verse Functions
int VerseAddChild(VerseInstance* v, PartInstance* p);
int VerseAddJump(VerseInstance* v, JumpInstance* j);
PartInstance* VerseFindFirstChild(VerseInstance* v, char* Name);
PartInstance* VerseFindFirstJump(VerseInstance* v, char* Name);




// Shader Functions
int BuildShader(GLenum type, char* filename);


#endif