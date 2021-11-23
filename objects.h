// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

#ifndef OBJECTS_H
#define OBJECTS_H

// DEPENDENCIES
#include "linmath.h"


// CLASSES
struct _GameInstance {
    char* Name;
    mat4x4 CFrame;
    int HomeVerse;
};
typedef struct _GameInstance GameInstance;

struct Verse {
    int Shared;
    char * Name;
    GameInstance* Children;
    int _childrenNum;
};
typedef struct _Verse Verse;

struct Jump {
    mat4x4 CFrame0, CFrame1;
    vec4 Norm0, Norm1;
    int Verse0, Verse1;
    double rad;
};
typedef struct _Jump Jump;

#endif