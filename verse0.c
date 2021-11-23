#include "linmath.h"
#include "game.h"

void cube(vec3 size, vec3 pos, vec3 orien) {
    glPushMatrix();
    // Place cube
    glTranslated(pos[0],pos[1],pos[2]);
    glRotated(orien[0], 1,0,0);
    glRotated(orien[1], 0,1,0);
    glRotated(orien[2], 0,0,1);
    glScaled(.5*size[0], .5*size[1], .5*size[2]);

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

int verse0() {


    return 0;
}