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


// SIMPLE DATATYPES
typedef GLubyte color3[3];


// GAME INSTANCES
// Parts
// @brief An Instance for a renderable object
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
    // Vertices

    // UV Mapping Data
    // Texture
    // Color & Lighting
    color3 Color;
    // Shader Properties
    // Collision Properties
};
typedef struct PartInstance_s PartInstance;


// Simple Objects / Primitive Building Objects
PartInstance* empty();
PartInstance* triangle(vec3 vert0, vec3 vert1, vec3 vert2);
PartInstance* plane(double size);
PartInstance* circle(int vertices, double radius);
PartInstance* cube(double size);
PartInstance* uvSphere(int segments, int rings, double radius);
PartInstance* icoSphere(int subdivisions, double radius);
PartInstance* cylinder(int vertices, double radius, double depth);
PartInstance* cone(int vertices, double rad1, double rad0, double depth);
PartInstance* wedge(double size);
PartInstance* wedgecorner(double size);

// Loadable Objects
PartInstance* meshFromObj(int* name, char* objpath);

// Custom Objects
    // NONE


// Part Methods
void NewInstance(PartInstance* p);
void SetColor(PartInstance* p, color3 c);
void SetCFrame(PartInstance* p, mat4x4 cf);
void SetHomeVerse(PartInstance* p, int hv);
void SetSize(PartInstance* p, vec3 s);
void SetPosition(PartInstance* p, vec3 pos);
void SetRotation(PartInstance* p, vec3 r);
void SetTranslation(PartInstance* p, vec3 size, vec3 position, vec3 rotation);
void SetMetaProperties(PartInstance* p, char* Name, char* ClassName, int HomeVerse);




// Jumps
// @brief An Instance that allows connecting multiple verses
struct JumpInstance_s {};
typedef struct JumpInstance_s JumpInstance;




// Verses
// @brief The Instance that describes a world (verse), containing part and jump instances
#define MAX_VERSE_INSTANCES 512
#define MAX_JUMP_INSTANCES 32
struct VerseInstance_s {
    // Properties
    int VerseID;
    int _Loaded;
    char* Name;
    mat4x4 ProjectMatrix;

    PartInstance* Children[MAX_VERSE_INSTANCES];
    JumpInstance* Jumps[MAX_JUMP_INSTANCES];

    // Methods
        // @brief Function to construct/ init the verse instance
        // @param self Verse Instance called from
        // @param window GLFW window context
    void (*Build) (struct VerseInstance_s, GLFWwindow*);
        // @brief Function to deconstruct/ cleanup the verse instance
        // @param self Verse Instance called from
        // @param window GLFW window context
    void (*Clean) (struct VerseInstance_s, GLFWwindow*);

    // Events
    SignalInstance RenderStepped;
};
typedef struct VerseInstance_s VerseInstance;


// Verse Functions
int VerseAddChild(VerseInstance v, PartInstance* p);
PartInstance* VerseFindFirstChild(VerseInstance v, char* Name);


#endif