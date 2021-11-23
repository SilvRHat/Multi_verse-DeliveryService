// Multi_verse DeliveryService 
// Gavin Zimmerman

// PANEL SOURCE CODE
    // Init / Management 
    // Game cycle
    // Define glfw callbacks
    // Manage window control


// DEPENDENCIES
#include "game.h"       // Graphics / Globals
#include "signal.h"     // Signals
#include "camera.h"     // Camera
#include "linmath.h"

#include "verse0.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct signal   KeyInput = NewSignal,
                MouseButtonInput = NewSignal,
                ScrollInput = NewSignal,
                CursorInput = NewSignal;



// SOURCE
void glfwErrorHandler(int code, const char* desc) {
    printf("ERROR %d: %s\n", code, (desc!=NULL)? desc: "GLFW Encountered an issue");
}


void windowResize(GLFWwindow* window, int width, int height) {
    double asp;
    
    glfwGetFramebufferSize(window, &width, &height);
    asp = (height>0)? (double)width/height : 1;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60, asp, 1/4, 20);
    double dim=5;
    //glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
    gluPerspective(60, asp, dim/8.0, 6*dim);
    glMatrixMode(GL_MODELVIEW);
};


// Input Callbacks
static void _KeyCallbackHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SignalFire(&KeyInput, window, key, scancode, action, mods);
}
static void _MouseCallbackHandler(GLFWwindow* window, int button, int action, int mods) {
    SignalFire(&MouseButtonInput, window, button, action, mods);
}
static void _ScrollCallbackHandler(GLFWwindow* window, double xoffset, double yoffset) {
    SignalFire(&ScrollInput, window, xoffset, yoffset);
}
static void _CursorCallbackHandler(GLFWwindow* window, double xpos, double ypos) {
    // Call Static Hanlders
    cameraCursorInput(window, xpos, ypos);
    // Signal Dynamic Handlers
    SignalFire(&CursorInput, window, xpos, ypos);
}



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

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    window = glfwCreateWindow(windowX, windowY, "Multi_verse DeliveryService", NULL, NULL);
    if (!window) exit(EXIT_FAILURE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    windowResize(window, windowX, windowY);
    
    // Window refresh callbacks
    glfwSetWindowSizeCallback(window, windowResize);
    // TODO     glfwSetWindowRefreshCallback(window, );
    
    // Input Callbacks
    glfwSetKeyCallback(window, _KeyCallbackHandler);
    glfwSetMouseButtonCallback(window, _MouseCallbackHandler);
    glfwSetScrollCallback(window, _ScrollCallbackHandler);
    glfwSetCursorPosCallback(window, _CursorCallbackHandler);

    // Init
    cameraInit(window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // Game Loop
    double t0 = glfwGetTime();
    while(!glfwWindowShouldClose(window)) {
        double t = glfwGetTime();
        double dt = t - t0;
        t0 = t;

        // Camera
        glLoadIdentity();
        cameraStep(window, t, dt);

        // Render Scene
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            cube((vec3){1,1,1}, (vec3){-2,0,0}, (vec3){0,0,0});
            cube((vec3){1,1,1}, (vec3){2,0,0}, (vec3){0,0,0});
            cube((vec3){1,1,1}, (vec3){0,-2,0}, (vec3){0,0,0});
            cube((vec3){1,1,1}, (vec3){0,2,0}, (vec3){0,0,0});
            cube((vec3){1,1,1}, (vec3){0,0,2}, (vec3){0,0,0});
            cube((vec3){1,1,1}, (vec3){0,0,-2}, (vec3){0,0,0});
            
        }
        {
            const double len=2.0;  //  Length of axes
            glBegin(GL_LINES);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(len,0.0,0.0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(0.0,len,0.0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(0.0,0.0,len);
            glEnd();
            //  Label axes
            
            //  Show quads
            glPushMatrix();
            
            glPopMatrix();
        }

        // Input
        //printf("A %lf\n", t-glfwGetTime()); t=glfwGetTime();
        
        glfwSwapBuffers(window);
        //printf("B %lf\n", t-glfwGetTime()); t=glfwGetTime();
        glfwPollEvents();
        //printf("C %lf\n", t-glfwGetTime()); t=glfwGetTime();
        
    }

    // Termination
    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}