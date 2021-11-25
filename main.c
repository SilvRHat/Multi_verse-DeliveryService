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
    double dim=10;
    //glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
    gluPerspective(60, asp, dim/8.0, 6*dim);
    glMatrixMode(GL_MODELVIEW);
}



void gameLoop(GLFWwindow* window) {
    ErrCheck("gameLoop BEGIN");
    static double t0 = 0;
    double t = glfwGetTime();
    double dt = t - t0;
    t0 = t;

    // Camera
    //glLoadIdentity(); // TODO Remove
    cameraStep(window, t, dt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render Scene
    /*{
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
    }*/
    renderStep(window, t, dt);
    
    // Swap Buffers
    glfwSwapBuffers(window);

    // Input
    glfwPollEvents();
    ErrCheck("gameLoop EXIT");
}


void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(EXIT_FAILURE);
}
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

void loadShader() {
    int prog = glCreateProgram();
    int vert = CreateShader(GL_VERTEX_SHADER, "default.vert");
    int frag = CreateShader(GL_FRAGMENT_SHADER, "default.frag");

    glAttachShader(prog,vert);
    glAttachShader(prog,frag);
    glLinkProgram(prog);
    PrintProgramLog(prog);


    DEFAULT_SHADER = prog;
}

void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
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
    window = glfwCreateWindow(windowX, windowY, "Multi_verse DeliveryService", NULL, NULL);
    if (!window) { 
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    //windowResize(window, windowX, windowY);
    
    // Window refresh callbacks
    glfwSetWindowSizeCallback(window, windowResize);
    //glfwSetWindowRefreshCallback(window, gameLoop);
    
    // Input Callbacks
    glfwSetKeyCallback(window, KeyCallbackHandler);
    glfwSetMouseButtonCallback(window, MouseCallbackHandler);
    glfwSetScrollCallback(window, ScrollCallbackHandler);
    glfwSetCursorPosCallback(window, CursorCallbackHandler);

    // Controller Systems
    loadShader();
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