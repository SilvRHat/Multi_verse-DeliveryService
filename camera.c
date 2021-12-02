// Multi_verse DeliveryService
// Gavin Zimmerman

// Camera Controller Source
#include "camera.h"
#include "objects.h"
#include "home.h"


// STATE GLOBALS
static CameraInstance Camera;


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
    ErrCheck("ERRCHK IN: cameraStep()");

    mat4x4 M;

    // Build Rotation
    mat4x4_identity(M);
    mat4x4_rotate_Y(M, M, Camera.az);
    mat4x4_rotate_X(M, M, Camera.ph);

    // Translate
    vec4_set(M[3], Camera.CFrame[3]);
    mat4x4_dup(Camera.CFrame, M);

    // Zoom out
    mat4x4_dup(Camera.ViewCFrame, M);
    mat4x4_translate_in_place(Camera.ViewCFrame, 0,0,Camera.Focus);
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
#ifdef DEVMODE
    if (
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) || 
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)
    ){
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            // Translate
            mat4x4 m;
            mat4x4_translate(m, -dx * Camera.Focus / 500.0, dy * Camera.Focus / 500.0, 0);
            mat4x4_mul(Camera.CFrame, Camera.CFrame, m);
        
        } else {
            // Rotate
            Camera.az -= dx * TO_RAD / 2.0;
            Camera.ph -= dy * TO_RAD / 2.0;
            Camera.ph = clamp(Camera.ph, -MAX_INCLINATION_ANG*TO_RAD, MAX_INCLINATION_ANG*TO_RAD);
            Camera.az = fmod(Camera.az, 360.0*TO_RAD); Camera.ph = fmod(Camera.ph, 360.0*TO_RAD);
    }}
#endif
    xpos0 = xpos;
    ypos0 = ypos;
}


void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset) {
    Camera.Focus = clamp(Camera.Focus - (yoffset/2.0), MIN_FOCUS, MAX_FOCUS);
}


void GetCameraView(mat4x4 CFrame, int* ViewVerse) {
    mat4x4_dup(CFrame, Camera.ViewCFrame);
    *ViewVerse = Camera.ViewVerse;
};


void GetCameraFocus(mat4x4 CFrame, int* HomeVerse) {
    mat4x4_dup(CFrame, Camera.CFrame);
    *HomeVerse = Camera.HomeVerse;
}