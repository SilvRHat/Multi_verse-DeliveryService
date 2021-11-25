// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"


// GLOBAL STATE


// SOURCE
//  Axes vertex data
/*const float axes_data[] =
{
   0,0,0,
   2,0,0,
   0,0,0,
   0,2,0,
   0,0,0,
   0,0,2,
};
static unsigned int axes_vao=0;

static void Axes()
{
   //  Select shader
   glUseProgram(DEFAULT_SHADER);
    static unsigned int vbo=0;
   //  Once initialized, just bind VAO
   if (axes_vao)
      glBindVertexArray(axes_vao);
   //  Initialize VAO and VBO
   else
   {
      //  Create axes VAO to bind attribute arrays
      glGenVertexArrays(1,&axes_vao);
      glBindVertexArray(axes_vao);

      //  Get buffer name
      
      glGenBuffers(1,&vbo);
      //  Bind VBO
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      //  Copy cube data to VBO
      glBufferData(GL_ARRAY_BUFFER,sizeof(axes_data),axes_data,GL_STATIC_DRAW);

      //  Bind arrays (vertex only)
   }
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    int loc = glGetAttribLocation(DEFAULT_SHADER,"Vertex");
    glVertexAttribPointer(loc,3,GL_FLOAT,0,12,(void*) 0);
    glEnableVertexAttribArray(loc);

    int verse;
    mat4x4 ProjectionMatrix, ViewMatrix;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 20);
    GetCameraView(ViewMatrix, &verse);

   //  Set Projection Matrix
   int id = glGetUniformLocation(DEFAULT_SHADER,"ProjectionMatrix");
   glUniformMatrix4fv(id,1,0,ProjectionMatrix);
   //  View matrix is the modelview matrix since model matrix is I
   id = glGetUniformLocation(DEFAULT_SHADER,"ModelViewMatrix");
   glUniformMatrix4fv(id,1,0,ViewMatrix);
   //  Set color
   const float White[] = {1,1,1,1};
   id = glGetUniformLocation(DEFAULT_SHADER,"Kd");
   glUniform4fv(id,1,White);

   //  Draw Axes
   glDrawArrays(GL_LINES,0,6);

   //  Release VAO and VBO
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}*/

static void Draw(PartInstance* part)
{
    //  Select shader
    int verse;
    mat4x4 ProjectionMatrix, ViewMatrix;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 20);
    GetCameraView(ViewMatrix, &verse);

    mat4x4_mul(ViewMatrix, ViewMatrix, part->CFrame);
    mat4x4_scale_aniso(ViewMatrix, ViewMatrix, part->Size[0], part->Size[1], part->Size[2]);

    glUseProgram(part->shaderId);
    glCullFace(GL_FRONT);
    //  Once initialized, just bind VAO
    glBindVertexArray(part->vao);
    glBindBuffer(GL_ARRAY_BUFFER,part->vbo);
   
    int loc = glGetAttribLocation(part->shaderId,"Vertex");
    glVertexAttribPointer(loc,4,GL_FLOAT,0,28,(void*) 0);
    glEnableVertexAttribArray(loc);
    
    //  Set Projection and View Matrix
    int id = glGetUniformLocation(part->shaderId,"ProjectionMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ProjectionMatrix);
    id = glGetUniformLocation(part->shaderId,"ModelViewMatrix");
    glUniformMatrix4fv(id,1,0,(const GLfloat *)ViewMatrix);

    //  Set color
    const float c[] = {part->Color[0]/255.0, part->Color[1]/255.0, part->Color[2]/255.0, 1};
    id = glGetUniformLocation(part->shaderId,"Kd");
    glUniform4fv(id, 1, (const GLfloat *) c);

    //  Draw Cube
    glDrawArrays(GL_TRIANGLES,0,part->vertices);

    //  Release VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}



void renderVerse(VerseInstance verse) {
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        PartInstance* part = verse.Children[i];
        if (part==NULL)
            break; 
        // Perform Object Culling
        // Draw Instance
        Draw(part);
    }
}


void renderInit(GLFWwindow* window) {
    // Initialize Verses
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
    
    //mat4x4 mv;
    //mat4x4_identity(mv);
    renderVerse(HOME_VERSE);
    //Axes();
    ErrCheck("ERRCHK RET: renderStep()");
}