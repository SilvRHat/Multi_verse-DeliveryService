// Multi_verse DeliveryService 
// Gavin Zimmerman

// Camera Controller
#ifndef CAMERA_H
#define CAMERA_H


// DEPENDENCIES
#include "game.h"
#include "signal.h"

#include <stdarg.h>
#include <stddef.h>


// CLASSES
struct camInst {
    char* Name;
    mat4x4 CFrame, ViewCFrame;
    int HomeVerse, ViewVerse;
    double Focus,   // Distance
           az,      // Azimuth Angle
           ph;      // Inclination Angle
};
typedef struct camInst CameraClass;


// CONFIG
#define MAX_INCLINATION_ANG 360
#define MIN_FOCUS 1
#define MAX_FOCUS 25


// MEMBERS
void cameraInit(GLFWwindow* window);
void cameraStep(GLFWwindow* window, double t, double step);
void cameraCursorInput(GLFWwindow* window, double xpos, double ypos);
void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset);

#endif