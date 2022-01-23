// Multi_verse DeliveryService 
// Gavin Zimmerman

// Camera Controller
#ifndef CAMERA_H
#define CAMERA_H


// DEPENDENCIES
#include "MvRE.h"
#include "multiverse.h"

#include <math.h>
#include <stdio.h>


// CONFIG
#define MAX_LENS 16
#define MAX_INCLINATION_ANG 80
#define MIN_FOCUS 1
#define MAX_FOCUS 120
#define CHARACTER_SPEED 16


// CLASSES
struct Camera_Instance_s {
    char* Name;
    mat4x4 CFrame, ViewCFrame;
    VerseInstance *HomeVerse, *ViewVerse;
    
    double Focus,   // Distance
           az,      // Azimuth Angle
           ph;      // Inclination Angle
};
typedef struct Camera_Instance_s CameraInstance;


// MEMBERS
void cameraInit(GLFWwindow* window);
void cameraStep(GLFWwindow* window, double t, double step);
void cameraCursorInput(GLFWwindow* window, double xpos, double ypos);
void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset);


#endif