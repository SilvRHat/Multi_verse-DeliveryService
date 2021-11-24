// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"


// GLOBAL STATE


// SOURCE
// Temporary function
void _renderCube(vec3 size, vec3 position, vec3 orientation) {
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


void drawInstance(PartInstance* p) {
    printf("Called on part with: [%s]\n", p->ClassName);
    if (strcmp(p->ClassName,"Cube")==0) {
        printf("Rendered\n");
        _renderCube(p->Size, p->Position, p->Rotation);
    }
}

void renderVerse(VerseInstance verse) {
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        if (verse.Children[i]==NULL)
            break; 
        // Perform Object Culling
        drawInstance(verse.Children[i]);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //mat4x4 mv;
    //mat4x4_identity(mv);
    renderVerse(HOME_VERSE);
}