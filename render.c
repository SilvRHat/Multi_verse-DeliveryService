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

    mat4x4 ProjectionMatrix, ViewMatrix;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 20);
    GetCameraView(ViewMatrix, &verse);

    mat4x4_mul(ViewMatrix, ViewMatrix, part->CFrame);
    mat4x4_scale_aniso(ViewMatrix, ViewMatrix, part->Size[0], part->Size[1], part->Size[2]);

    glUseProgram(shdrprog);
    glCullFace(GL_FRONT);

    //  Once initialized, just bind VAO
    glBindVertexArray(vao);
   
    //  Set Projection and View Matrix
    int id = glGetUniformLocation(shdrprog,"ProjectionMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ProjectionMatrix);
    id = glGetUniformLocation(shdrprog,"ModelViewMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ViewMatrix);

    //  Set color
    id = glGetUniformLocation(shdrprog, "Kd");
    glUniform4fv(id, 1, (const GLfloat *) part->Color);

    // Call instance drawing operations
    part->_drawFunc(part);

    //  Release VAO and VBO
    //glBindVertexArray(0);
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
    MULTIVERSE.Build(MULTIVERSE, window);
    HOME_VERSE.Build(HOME_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(HOME_VERSE, window);
    MULTIVERSE.Clean(MULTIVERSE, window);
}


void renderStep(GLFWwindow* window, double t, double step) {
    ErrCheck("ERRCHK IN: renderStep()");
    renderVerse(HOME_VERSE);
    ErrCheck("ERRCHK RET: renderStep()");
}