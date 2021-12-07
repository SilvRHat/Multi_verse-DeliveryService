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
    Camera.Name = "CurrentCamera";
    Camera.HomeVerse = &HOME_VERSE;
    Camera.ViewVerse = &HOME_VERSE;
    for (int i=0; i<MAX_LENS; i++) {
        Camera.Lens[i].Jump = NULL;
        vec3_set(Camera.Lens[i].ViewPosition, VEC3_ZERO);
    }

    mat4x4_translate(Camera.CFrame, 0, 0, 0);
    cameraStep(window, 0, 0);
}


void cameraStep(GLFWwindow* window, double t, double step) {
    mat4x4 M, ZoomFrame;

    // Build Rotation
    mat4x4_identity(M);
    mat4x4_rotate_Y(M, M, Camera.az);
    mat4x4_rotate_X(M, M, Camera.ph);

    // Translate
    vec4_set(M[3], Camera.CFrame[3]);
    mat4x4_dup(Camera.CFrame, M);

    // Zoom out
    // TODO: ADD IN LENS (If it crosses a jump, make translation, subtract dist, repeat)
    mat4x4_dup(ZoomFrame, M);
    /*for (int i=0; i<MAX_LENS; i++) {
        JumpLens* lens = &Camera.Lens[i];
        if (lens->Jump == NULL)
            break;

        vec3 hit;
        vec4 campos, camlook, jumppos, jumplook;
        mat4x4_col(camlook, ZoomFrame, 2); 
        mat4x4_col(campos, ZoomFrame, 3); 
        
        line_plane_intersection(hit, campos, camlook, );
    }*/
    
    mat4x4_translate_in_place(ZoomFrame, 0,0,Camera.Focus);
    mat4x4_dup(Camera.ViewCFrame, ZoomFrame);
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
        }
    }
#endif
    xpos0 = xpos;
    ypos0 = ypos;
}


void cameraScrollInput(GLFWwindow* window, double xoffset, double yoffset) {
    Camera.Focus = clamp(Camera.Focus - (yoffset/2.0), MIN_FOCUS, MAX_FOCUS);
}


void GetCameraHome(mat4x4 CFrame, VerseInstance** verse) {
    mat4x4_dup(CFrame, Camera.CFrame);
    *verse = Camera.HomeVerse;
}


void GetCameraView(mat4x4 CFrame, VerseInstance** verse) {
    mat4x4_dup(CFrame, Camera.ViewCFrame);
    *verse = Camera.ViewVerse;
};