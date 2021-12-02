// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"



// SOURCE
void DrawInstance(PartInstance* part) {
    //  Select shader
    int verse;
    unsigned int vao = part->vao, 
        shdrprog = part->shader;

    mat4x4 ProjectionMatrix, ViewMatrix, CameraViewM;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 10000);
    GetCameraView(CameraViewM, &verse);

    // Camera and all pat instances exist in same coordinate space
    // Need to find offset between camera and part
    mat4x4_invert(ViewMatrix, CameraViewM);
    mat4x4_mul(ViewMatrix, ViewMatrix, part->CFrame);

    // Apply Sizing to part // TODO: Export to shader as seperate
    mat4x4_scale_aniso(ViewMatrix, ViewMatrix, part->Size[0], part->Size[1], part->Size[2]);
    glUseProgram(shdrprog);

    //  Bind VAO
    glBindVertexArray(vao);
   
    //  Set Projection and View Matrix
    int id = glGetUniformLocation(shdrprog,"ProjectionMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ProjectionMatrix);
    id = glGetUniformLocation(shdrprog,"ModelViewMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ViewMatrix);

    //  Set color
    vec4 c = {0,0,0,1};
    for (int i=0; i<3; i++) c[i] = part->Color[i]/255.0;


    id = glGetUniformLocation(shdrprog, "Kd");
    glUniform4fv(id, 1, (const GLfloat *) c);

    // Call instance drawing operations
    part->_drawFunc(part);
}



void renderVerse(VerseInstance verse) {
    for (int i=0; i<MAX_INSTANCES; i++) {
        PartInstance* part = verse.Children[i];
        if (part==NULL)
            break; 
        // Perform Object Culling
        // Draw Instance
        DrawInstance(part);
    }
}




void renderInit(GLFWwindow* window) {
    // Initialize Verses
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    MULTIVERSE.Build(&MULTIVERSE, window);
    HOME_VERSE.Build(&HOME_VERSE, window);
    GATE_VERSE.Build(&GATE_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(&HOME_VERSE, window);
    MULTIVERSE.Clean(&MULTIVERSE, window);
    GATE_VERSE.Clean(&GATE_VERSE, window);
    
}


void renderStep(GLFWwindow* window, double t, double step) {
    ErrCheck("ERRCHK IN: renderStep()");
    //renderVerse(GATE_VERSE);
    renderVerse(HOME_VERSE);
    glBindVertexArray(0);
    ErrCheck("ERRCHK RET: renderStep()");
}