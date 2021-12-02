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


void gameLoop(GLFWwindow* window) {
    ErrCheck("ERRCHK IN: gameLoop()");
    static double t0 = 0;
    double t = glfwGetTime();
    double dt = t - t0;
    t0 = t;

    // Camera
    cameraStep(window, t, dt);

    // Render Scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderStep(window, t, dt);
    
    // Swap Buffers
    glfwSwapBuffers(window);

    // Input
    glfwPollEvents();
    ErrCheck("ERRCHK RET: gameLoop()");
}


// Error Functions
// Credit to Willem A. (Vlakkies) Schreuder
void Error(const char* format , ...) {
   va_list args;
   va_start(args,format);
   printf("ERROR: ");
   vfprintf(stderr,format,args);
   va_end(args);
   exit(EXIT_FAILURE);
}


void ErrCheck(const char* where) {
   int err = glGetError();
   char* desc = "No Error";
   switch (err) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            desc = "An unacceptable value is specified for an enumerated argument.";
            break;
        case GL_INVALID_VALUE:
            desc = "A numeric argument is out of range.";
            break;
        case GL_INVALID_OPERATION:
            desc = "The specified operation is not allowed in the current state.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            desc = "The framebuffer object is not complete.";
            break;
        case GL_OUT_OF_MEMORY:
            desc = "There is not enough memory left to execute the command.";
            break;
        default:
            desc = "Unknown Error";
            break;
   }
   if (err) fprintf(stderr,"ERROR: %s \nFrom: (%s)\n",desc,where);
}

void glfwErrorHandler(int code, const char* desc) {
    printf("ERROR %d: %s\n", code, (desc!=NULL)? desc: "GLFW Encountered an issue");
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(windowX, windowY, "Multi_verse DeliveryService - Gavin Zimmerman", NULL, NULL);
    if (!window) { 
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // Input Callbacks
    glfwSetKeyCallback(window, KeyCallbackHandler);
    glfwSetMouseButtonCallback(window, MouseCallbackHandler);
    glfwSetScrollCallback(window, ScrollCallbackHandler);
    glfwSetCursorPosCallback(window, CursorCallbackHandler);

    // Controller Systems
    cameraInit(window); 
    renderInit(window);

    // OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // Game Loop 
    while(!glfwWindowShouldClose(window)) {
        gameLoop(window);
    }

    // Termination
    renderExit(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}