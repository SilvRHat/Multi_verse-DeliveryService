// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"


// GLOBAL STATE


// SOURCE
// Temporary function
void _cube(vec3 size, vec3 position, vec3 rotation) {
    glPushMatrix();
    // Place cube
    glTranslated(position[0],position[1],position[2]);
    glRotated(rotation[1], 0,1,0);
    glRotated(rotation[0], 1,0,0);
    glRotated(rotation[2], 0,0,1);
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

//  Axes vertex data
const float axes_data[] =
{
   0,0,0,
   2,0,0,
   0,0,0,
   0,2,0,
   0,0,0,
   0,0,2,
};
static unsigned int axes_vao=0;
/*
 * Draw axes
 */
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
}

static void Draw(PartInstance* part)
{
    //  Select shader
    int verse;
    mat4x4 ProjectionMatrix, ViewMatrix;
    mat4x4_perspective(ProjectionMatrix, 60*TO_RAD, 1080.0/720.0, .1, 20);
    GetCameraView(ViewMatrix, &verse);

    mat4x4_mul(ViewMatrix, ViewMatrix, part->CFrame);
    mat4x4_scale_aniso(ViewMatrix, ViewMatrix, part->Size[0], part->Size[1], part->Size[2]);

    glUseProgram(DEFAULT_SHADER);
    glCullFace(GL_FRONT);
    //  Once initialized, just bind VAO
    glBindVertexArray(part->vao);
    glBindBuffer(GL_ARRAY_BUFFER,part->vbo);
   
    int loc = glGetAttribLocation(DEFAULT_SHADER,"Vertex");
    glVertexAttribPointer(loc,4,GL_FLOAT,0,28,(void*) 0);
    glEnableVertexAttribArray(loc);
    
    //  Set Projection and View Matrix
    int id = glGetUniformLocation(DEFAULT_SHADER,"ProjectionMatrix");
    glUniformMatrix4fv(id,1,0,ProjectionMatrix);
    id = glGetUniformLocation(DEFAULT_SHADER,"ModelViewMatrix");
    glUniformMatrix4fv(id,1,0,ViewMatrix);

    //  Set color
    const float c[] = {part->Color[0]/255.0, part->Color[1]/255.0, part->Color[2]/255.0, 1};
    id = glGetUniformLocation(DEFAULT_SHADER,"Kd");
    glUniform4fv(id, 1, c);

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
    HOME_VERSE.Build(HOME_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(HOME_VERSE, window);
}


void renderStep(GLFWwindow* window, double t, double step) {
    ErrCheck("renderStep BEGIN");
    
    //mat4x4 mv;
    //mat4x4_identity(mv);
    renderVerse(HOME_VERSE);
    Axes();
    ErrCheck("renderStep END");
}