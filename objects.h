// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

#ifndef OBJECTS_H
#define OBJECTS_H

// DEPENDENCIES
#include "linmath.h"
#include "signal.h"




// OBJECTS
// The defined pieces that will allow a game to be made up

// Simple datatypes
typedef GLubyte color3[3];



// Part Instances
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

// Simple Object Constructors
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

// Loadable Object
PartInstance* meshFromObj(int* name, char* objpath);

// Custom Objects
    // NONE


// PartInstance Methods
void NewInstance(PartInstance* p);
void SetColor(PartInstance* p, color3 c);
void SetCFrame(PartInstance* p, mat4x4 cf);
void SetHomeVerse(PartInstance* p, int hv);
void SetSize(PartInstance* p, vec3 s);
void SetPosition(PartInstance* p, vec3 pos);
void SetRotation(PartInstance* p, vec3 r);
void SetTranslation(PartInstance* p, vec3 size, vec3 position, vec3 rotation);
void SetMetaProperties(PartInstance* p, char* Name, char* ClassName, int HomeVerse);

void RenderPartInstance(PartInstance* p);



// Jumps
struct JumpInstance_s {};
typedef struct JumpInstance_s JumpInstance;




// Verse Instances
#define MAX_VERSE_INSTANCES 512
#define MAX_JUMP_INSTANCES 32
struct VerseInstance_s {
    int VerseID;
    char* Name;

    void (*_RenderFunc) (GLFWwindow*, struct VerseInstance_s, double, double);
    signal RenderStepped;

    PartInstance* Children[MAX_VERSE_INSTANCES];
    JumpInstance* Jumps[MAX_JUMP_INSTANCES];
};
typedef struct VerseInstance_s VerseInstance;


// Verse Instance Methods
void SetRenderFunc(VerseInstance v, void (*RenderFunc) (GLFWwindow*, struct VerseInstance_s, double, double));
void Render(VerseInstance v);

void AddChild(VerseInstance v, PartInstance* p);
PartInstance* FindFirstChild(VerseInstance v, char* Name);

void BuildVerse(VerseInstance v);
void ClearVerse(VerseInstance v);



#endif