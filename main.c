// Multi_verse DeliveryService 
// Gavin Zimmerman

// Main / Panel Source
// DEPENDENCIES
#include "main.h"


SignalInstance  KeyInput = NewSignal,
                MouseButtonInput = NewSignal,
                ScrollInput = NewSignal,
                CursorInput = NewSignal,
                FrameBufferSize = NewSignal;


// SOURCE
// Input Callbacks
static void KeyCallbackHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SignalFire(&KeyInput, window, key, scancode, action, mods);
}


static void MouseCallbackHandler(GLFWwindow* window, int button, int action, int mods) {
    SignalFire(&MouseButtonInput, window, button, action, mods);
}


static void ScrollCallbackHandler(GLFWwindow* window, double xoffset, double yoffset) {
    // Call Static Handlers
    cameraScrollInput(window, xoffset, yoffset);
    // Signal Dynamic Handlers
    SignalFire(&ScrollInput, window, xoffset, yoffset);
}


static void CursorCallbackHandler(GLFWwindow* window, double xpos, double ypos) {
    // Call Static Handlers
    cameraCursorInput(window, xpos, ypos);
    // Signal Dynamic Handlers
    SignalFire(&CursorInput, window, xpos, ypos);
}

static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Call Static Handlers
    // Signal Dynamic Handlers
    SignalFire(&FrameBufferSize, window, width, height);
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
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderStep(window, t, dt);
    
    // Swap Buffers
    glfwSwapBuffers(window);

    // Input
    glfwPollEvents();
    ErrCheck("ERRCHK RET: gameLoop()");
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
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    // Controller Systems
    cameraInit(window); 
    renderInit(window);
    SignalFire(&FrameBufferSize, window, windowX, windowY);

    // OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Game Loop 
    MULTI_VERSE.Build(window);
    while(!glfwWindowShouldClose(window)) {
        gameLoop(window);
    }
    MULTI_VERSE.Clean(window);

    // Termination
    SignalDestroy(&KeyInput);
    SignalDestroy(&MouseButtonInput);
    SignalDestroy(&ScrollInput);
    SignalDestroy(&CursorInput);
    SignalDestroy(&FrameBufferSize);
    
    renderExit(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}