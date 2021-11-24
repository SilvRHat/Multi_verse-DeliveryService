// Multi_verse DeliveryService 
// Gavin Zimmerman

// Main / Panel Source
// DEPENDENCIES
#include "game.h"       // Graphics / Globals
#include "signal.h"     // Signals
#include "camera.h"     // Camera
#include "linmath.h"    // Vectors/ Matrices
#include "render.h"     // Render world

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


SignalInstance  KeyInput = NewSignal,
                MouseButtonInput = NewSignal,
                ScrollInput = NewSignal,
                CursorInput = NewSignal;


// SOURCE
// Input Callbacks
static void KeyCallbackHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SignalFire(&KeyInput, window, key, scancode, action, mods);
}


static void MouseCallbackHandler(GLFWwindow* window, int button, int action, int mods) {
    SignalFire(&MouseButtonInput, window, button, action, mods);
}


static void ScrollCallbackHandler(GLFWwindow* window, double xoffset, double yoffset) {
    // Call Static Hanlders
    cameraScrollInput(window, xoffset, yoffset);
    // Signal Dynamic Handlers
    SignalFire(&ScrollInput, window, xoffset, yoffset);
}


static void CursorCallbackHandler(GLFWwindow* window, double xpos, double ypos) {
    // Call Static Hanlders
    cameraCursorInput(window, xpos, ypos);
    // Signal Dynamic Handlers
    SignalFire(&CursorInput, window, xpos, ypos);
}


// GLFW / Window Callback
void glfwErrorHandler(int code, const char* desc) {
    printf("ERROR %d: %s\n", code, (desc!=NULL)? desc: "GLFW Encountered an issue");
}


void windowResize(GLFWwindow* window, int width, int height) {
    double asp;
    // TODO : Replace with by window projection matrix
    glfwGetFramebufferSize(window, &width, &height);
    asp = (height>0)? (double)width/height : 1;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double dim=5;
    glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
    //gluPerspective(60, asp, dim/8.0, 6*dim);
    glMatrixMode(GL_MODELVIEW);
}

void _cube(vec3 position, vec3 orientation, vec3 size) {
    glPushMatrix();
    // Place cube
    glTranslated(position[0],position[1],position[2]);
    glRotated(orientation[1], 0,1,0);
    glRotated(orientation[0], 1,0,0);
    glRotated(orientation[2], 0,0,1);
    glScaled(size[0]/2.0, size[1]/2.0, size[2]/2.0);

    glBegin(GL_QUADS);
    //  Front
    glColor3ub(255, 0, 0);
    glVertex3f(-1,-1, 1);
    glVertex3f(+1,-1, 1);
    glVertex3f(+1,+1, 1);
    glVertex3f(-1,+1, 1);
    //  Back
    glColor3ub(255, 255, 0);
    glVertex3f(+1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,+1,-1);
    glVertex3f(+1,+1,-1);
    //  Right
    glColor3ub(0, 255, 0);
    glVertex3f(+1,-1,+1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,+1,-1);
    glVertex3f(+1,+1,+1);
    //  Left
    glColor3ub(0, 255, 255);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,+1);
    glVertex3f(-1,+1,+1);
    glVertex3f(-1,+1,-1);
    //  Top
    glColor3ub(0, 0, 255);
    glVertex3f(-1,+1,+1);
    glVertex3f(+1,+1,+1);
    glVertex3f(+1,+1,-1);
    glVertex3f(-1,+1,-1);
    //  Bottom
    glColor3ub(255, 0, 255);
    glVertex3f(-1,-1,-1);
    glVertex3f(+1,-1,-1);
    glVertex3f(+1,-1,+1);
    glVertex3f(-1,-1,+1);
    //  End
    glEnd();
    glPopMatrix();
}

void gameLoop(GLFWwindow* window) {
    static double t0 = 0;
    double t = glfwGetTime();
    double dt = t - t0;
    t0 = t;

    // Camera
    glLoadIdentity(); // TODO Remove
    //cameraStep(window, t, dt);

    // Render Scene
    {
        _cube((vec3) { 0, 0,-1}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
        _cube((vec3) { 0, 0, 1}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
        _cube((vec3) { 0,-1, 0}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
        _cube((vec3) { 0, 1, 0}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
        _cube((vec3) {-1, 0, 0}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
        _cube((vec3) { 1, 0, 0}, (vec3) {0.0,0.0,0.0}, (vec3) {1.0,1.0,1.0});
    }
    //renderStep(window, t, dt);
    
    // Input
    glfwPollEvents();

    // Swap Buffers
    glfwSwapBuffers(window);
}



// Main
int main() {
    GLFWwindow* window;
    int windowX = 1080, windowY = 720;

    // Init
#ifdef USEGLEW
    if (glewInit()!=GLEW_OK) exit(-1);
#endif
    if (!glfwInit()) {
        printf("    Error 0: Could not initialize glfw3\n");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(glfwErrorHandler);

    // Window creation
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(windowX, windowY, "Multi_verse DeliveryService", NULL, NULL);
    if (!window) { 
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    
    // Window refresh callbacks
    glfwSetWindowSizeCallback(window, windowResize);
    //glfwSetWindowRefreshCallback(window, gameLoop);
    
    // Input Callbacks
    glfwSetKeyCallback(window, KeyCallbackHandler);
    glfwSetMouseButtonCallback(window, MouseCallbackHandler);
    glfwSetScrollCallback(window, ScrollCallbackHandler);
    glfwSetCursorPosCallback(window, CursorCallbackHandler);

    // OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Controller Systems
    cameraInit(window);
    //renderInit(window);

    // Game Loop 
    glfwSwapInterval(1);
    windowResize(window, windowX, windowY);
    while(!glfwWindowShouldClose(window)) {
        gameLoop(window);
    }

    // Termination
    //renderExit(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}