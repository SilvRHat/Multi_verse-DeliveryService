// Multi_verse DeliveryService 
// Gavin Zimmerman

// Camera Controller
#ifndef CAMERA_H
#define CAMERA_H


// DEPENDENCIES
#include "linmath.h"
#include "game.h"
#include "camera.h"
#include "objects.h"

#include <math.h>
#include <stdio.h>



// CLASSES
struct Camera_Instance_s {
    char* Name;
    mat4x4 CFrame, ViewCFrame;
    int HomeVerse, ViewVerse;
    double Focus,   // Distance
           az,      // Azimuth Angle
           ph;      // Inclination Angle
};
typedef struct Camera_Instance_s CameraInstance;


// CONFIG
#define MAX_INCLINATION_ANG 80
#define MIN_FOCUS 1
#define MAX_FOCUS 25


// MEMBERS
void cameraInit(GLFWwindow* window);
void cameraStep(GLFWwindow* window, double t, double step);
void cameraCursorInput(GLFWwindow* window, double xpos, double ypos);
void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset);
void GetCameraView(mat4x4 CFrame, int* ViewVerse);
void GetCameraFocus(mat4x4 CFrame, int* HomeVerse);

#endif