// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"



// SOURCE
void DrawInstance(PartInstance* part) {
    int verse;
    unsigned int vao = part->Vao, 
        shdrprog = part->Shader;

    // Check if drawing properties are set
    if (!(part->Vao && part->Vbo && part->Shader)) 
        return;

    mat4x4 ProjectionMatrix, ViewMatrix, CameraViewM;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 10000);   // TODO Make resizeable
    GetCameraView(CameraViewM, &verse);

    // Camera and all part instances exist in same coordinate space; get part offset from camera
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
    vec4 c = {0,0,0,part->Transparency};
    for (int i=0; i<3; i++) c[i] = part->Color[i]/255.0;


    id = glGetUniformLocation(shdrprog, "Color");
    glUniform4fv(id, 1, (const GLfloat *) c);

    // Call instance drawing operations
    part->Draw(part);
}



void renderVerse(VerseInstance verse) {
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
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
    // Build Verses
    MULTI_VERSE.Build(&MULTI_VERSE, window);
    HOME_VERSE.Build(&HOME_VERSE, window);
    GATE_VERSE.Build(&GATE_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(&HOME_VERSE, window);
    MULTI_VERSE.Clean(&MULTI_VERSE, window);
    GATE_VERSE.Clean(&GATE_VERSE, window);  
}


void renderStep(GLFWwindow* window, double t, double step) {
    ErrCheck("ERRCHK IN: renderStep()");
    // renderVerse(GATE_VERSE);
    renderVerse(HOME_VERSE);
    glBindVertexArray(0);
    ErrCheck("ERRCHK RET: renderStep()");
}