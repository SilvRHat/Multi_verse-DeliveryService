// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

// Camera Controller


// DEPENDENCIES
#include "linmath.h"
#include "game.h"
#include "camera.h"
#include "objects.h"
#include "camera.h"

#include <math.h>
#include <stdio.h>


// STATE GLOBALS
private CameraClass Camera;



// SOURCE
void cameraInit(GLFWwindow* window) {
    Camera.Focus = 10;
    Camera.az = 0;
    Camera.ph = 0;
    Camera.HomeVerse = 0;
    Camera.ViewVerse = 0;
    Camera.Name = "CurrentCamera";

    mat4x4_translate(Camera.CFrame, 0, 0, 0);
    mat4x4_dup(Camera.ViewCFrame, Camera.CFrame);
    mat4x4_translate_in_place(Camera.ViewCFrame, 0,0, -Camera.Focus);
}


void cameraStep(GLFWwindow* window, double t, double step) {
    vec4 cam_pos;
    mat4x4 cam_rot, m;

    // TODO Character Movement
    //double forward = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    //double right = glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D);

    // Reset orientation / Save translation
    mat4x4_col(cam_pos, Camera.CFrame, 3);
    mat4x4_translate(Camera.CFrame, cam_pos[0], cam_pos[1], cam_pos[2]);
    mat4x4_rotate_X(Camera.CFrame, Camera.CFrame, -90 * TO_RAD);
    
    // Build Rotation
    mat4x4_identity(cam_rot);
    mat4x4_identity(m);

    mat4x4_rotate_Z(cam_rot, cam_rot, Camera.az);
    mat4x4_rotate_X(m, m, Camera.ph);
    mat4x4_mul(cam_rot, m, cam_rot);
    mat4x4_orthonormalize(cam_rot, cam_rot);
    mat4x4_mul(Camera.CFrame, Camera.CFrame, cam_rot);

    // Build View Frame (Zoom out)
    mat4x4_dup(Camera.ViewCFrame, Camera.CFrame);
    mat4x4_translate(m,0,0,-Camera.Focus);
    mat4x4_mul(Camera.ViewCFrame, m, Camera.ViewCFrame);
    glMultMatrixf((GLfloat*) Camera.ViewCFrame);
}


void cameraCursorInput(GLFWwindow* window, double xpos, double ypos) {
    static double xpos0, ypos0;
    double dx, dy;
    static int init=0;
    if (!init) { init=1;
        glfwGetCursorPos(window, &xpos0, &ypos0);
    }

    dx = xpos - xpos0;
    dy = ypos - ypos0;
    if ((DEVMODE) && (   
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || 
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)
        )
    ){
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            // Translate
            mat4x4 m;
            mat4x4_translate(m, dx * Camera.Focus / 500.0, -dy * Camera.Focus / 500.0, 0);
            mat4x4_mul(Camera.CFrame, m, Camera.CFrame);
        
        } else {
            // Rotate
            Camera.az += dx * TO_RAD / 2.0;
            Camera.ph += dy * TO_RAD / 2.0;
            Camera.ph = clamp(Camera.ph, -MAX_INCLINATION_ANG*TO_RAD, MAX_INCLINATION_ANG*TO_RAD);
            Camera.az = fmod(Camera.az, 360.0*TO_RAD); Camera.ph = fmod(Camera.ph, 360.0*TO_RAD);
    }}
    xpos0 = xpos;
    ypos0 = ypos;
}

void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset) {
    Camera.Focus = clamp(Camera.Focus - (yoffset/2.0), MIN_FOCUS, MAX_FOCUS);
}