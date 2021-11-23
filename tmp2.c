// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

// Camera Controller

#include "game.h"
#include "camera.h"
#include "objects.h"
#include "camera.h"
#include "linmath.h"
#include <math.h>

#include<stdio.h>

#define TO_RAD M_PI/180.0
#define TO_DEG 180.0/M_PI


// STATE GLOBALS
struct camInst {
    char* Name;
    mat4x4 CFrame, ViewCFrame;
    int HomeVerse, ViewVerse;
    double Focus,   // Distance
           az,      // Azimuth Angle
           ph;      // Inclination Angle
};
typedef struct camInst CameraClass;
CameraClass Camera;

// CONFIG
#define MAX_INCLINATION_ANG 360
#define MIN_FOCUS 1
#define MAX_FOCUS 25




void cameraCursorInput(va_list args);
void cameraKeyInput(va_list args);




// SOURCE
void cameraInit(GLFWwindow* window) {
    Camera.Focus = 10;
    Camera.az = 0;
    Camera.HomeVerse = 0;
    Camera.ViewVerse = 0;

    Camera.Name = "CurrentCamera";
    mat4x4_translate(Camera.CFrame, 0, 0, 0);
    //mat4x4_rotate_Y(Camera.CFrame, Camera.CFrame, 45);
    //mat4x4_rotate_X(Camera.CFrame, Camera.CFrame, 40);
    mat4x4_dup(Camera.ViewCFrame, Camera.CFrame);
    mat4x4_translate_in_place(Camera.ViewCFrame, 0,0, -Camera.Focus)

    SignalConnect(&KeyInput, cameraKeyInput);
    SignalConnect(&CursorInput, cameraCursorInput);
}


void cameraStep(GLFWwindow* window, double t, double step) {
    double forward = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    double right = glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D);
    glTranslated(0,0,-10);
    glMultMatrixf((GLfloat*) Camera.CFrame);
    
}


void cameraKeyInput(va_list args) {
    GLFWwindow* window = va_arg(args, GLFWwindow*);
    int key = va_arg(args, int);
    int scancode = va_arg(args, int); 
    int action = va_arg(args, int);
    int mods = va_arg(args, int);
}


void cameraCursorInput(va_list args) {
    GLFWwindow* window = va_arg(args, GLFWwindow*);
    double xpos= va_arg(args, double);
    double ypos= va_arg(args, double);
    
    

    static double xpos0 = 0, ypos0 = 0;
    static int init=0;
    if (!init) {
        init=1;
        glfwGetCursorPos(window, &xpos0, &ypos0);
    }


    double dx = xpos - xpos0, dy = ypos - ypos0;
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        // Translate
        printf("ILLEGAL\n");
    } else {
        // Rotate
        mat4x4 r, b;
        mat4x4_identity(r);
        mat4x4_rotate_Z(r, r, dx * TO_RAD / 2.0);
        mat4x4_invert(r,r);
        mat4x4_mul(Camera.CFrame, Camera.CFrame, r);
        mat4x4_orthonormalize(Camera.CFrame, Camera.CFrame);

        mat4x4_identity(r);
        mat4x4_rotate_X(r, r, dy * TO_RAD / 2.0);
        
        mat4x4_mul(Camera.CFrame, r, Camera.CFrame);
        mat4x4_orthonormalize(Camera.CFrame, Camera.CFrame);
    }}
    xpos0 = xpos; 
    ypos0 = ypos;
    
}

