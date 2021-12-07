// Multi_verse DeliveryService
// Gavin Zimmerman

// Objects
#ifndef OBJECTS_H
#define OBJECTS_H


// DEPENDENCIES
#include "game.h"
#include "linmath.h"
#include "signal.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// CONST
#define VERTEX_DATA_ROW 7
#define MAX_PART_INSTANCES 512  // TODO: Make dyanmic
#define MAX_JUMP_INSTANCES 64


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

#define DRAWMODE_FILLED 0
#define DRAWMODE_LINE   1

// Shader Layout
#define ATTRIB_POS_VERTEX           5
#define ATTRIB_POS_NORMAL           6



// SIMPLE DATATYPES
typedef GLubyte color3[3];


// GAME INSTANCES
struct PartInstance_s {
    // Book Keeping
    char* Name;
    char* ClassName;
    // Positional/ Scaling
    mat4x4 CFrame;
    vec3 Size;
    vec3 Position;
    vec3 Rotation;
    // Vertices Data
    GLuint Vao, Vbo;
    unsigned int Triangles, Vertices;
    void (*Draw)();    // Replace for part instancing
    // UV Mapping Data
    // Texture
    // Color & Lighting
    color3 Color;
    float Transparency;
    GLuint Shader;
    // Collision Properties
};
typedef struct PartInstance_s PartInstance;



struct JumpInstance_s {
    // Book Keeping
    char *Name;
    char *ClassName;

    // Jumping
    void *Verse0, *Verse1;
    mat4x4 CFrame0, CFrame1;
    mat4x4 _ToV0, _ToV1; 
    float Radius;
};
typedef struct JumpInstance_s JumpInstance;



struct VerseInstance_s {
    // Properties
    char* Name;
    mat4x4 ProjectMatrix;

    PartInstance* Children[MAX_PART_INSTANCES];
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
void PartSetShader(PartInstance* p, GLuint shader);




// Jump Functions
// Constructors / Destructors
JumpInstance* NewJumpInstance();
void *DestroyJumpInstance(JumpInstance* j);
void JumpConnectVerses(JumpInstance* j, VerseInstance* v0, VerseInstance* v1);
void JumpSetCFrames(JumpInstance* j, mat4x4 CF0, mat4x4 CF1);




// Verse Functions
int VerseAddPart(VerseInstance* v, PartInstance* p);
int VerseAddJump(VerseInstance* v, JumpInstance* j);
int VerseRemovePart(VerseInstance* v, PartInstance* p);
int VerseRemoveJump(VerseInstance* v, JumpInstance* j);
PartInstance* VerseFindFirstPart(VerseInstance* v, char* Name);
JumpInstance* VerseFindFirstJump(VerseInstance* v, char* Name);




// Shader Functions
GLuint BuildShaderFromFile(GLenum type, char* filename);


#endif