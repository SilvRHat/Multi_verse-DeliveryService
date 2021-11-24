// Multi_verse DeliveryService
// Gavin Zimmerman

// Objects Source
#include "objects.h"


// SOURCE
PartInstance* empty() {
    PartInstance* inst = (PartInstance*) malloc(sizeof(PartInstance));
    NewInstance(inst);
    return inst;
}


PartInstance* triangle(vec3 vert0, vec3 vert1, vec3 vert2) {
    // TODO
}


PartInstance* plane(double size) {
    PartInstance* inst = empty();
    inst->ClassName="Plane";
    // Generate vertices

    return inst;
}


PartInstance* circle(int vertices, double radius) {
    PartInstance* inst = empty();
    inst->ClassName="Circle";
    // Generate vertices

    return inst;
}


PartInstance* cube(double size) {
    PartInstance* inst = empty();
    inst->ClassName="Cube";
    // Generate vertices

    return inst;
}




// PartInstance Methods
void NewInstance(PartInstance* p) {
    p->Name="";
    p->ClassName="";
    mat4x4_identity(p->CFrame);
    p->HomeVerse=0;
    vec3_zero(p->Size);
    vec3_zero(p->Position);
    vec3_zero(p->Rotation);
    for (int i=0; i<3; i++) 
        p->Color[i]=255;
}


void SetColor(PartInstance* p, color3 c) {
    for (int i=0; i<3; i++) 
        p->Color[i]=c[i];
}


void SetCFrame(PartInstance* p, mat4x4 cf) {
    vec4 pos, rot;
    float rx, ry, rz;

    // TODO
    // Find YXZ Euler Angles from Rotation Matrix
    // Credit: http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
    if (1 - abs(cf[0][2]) < 1e-3) {

    }
    else { 
        // Gimbal lock edge case

    }



    mat4x4_dup(p->CFrame, cf);
    mat4x4_col(pos, cf, 3);
    for (int i=0; i<3; i++) {
        p->Position[i]=pos[i];
        p->Rotation[i]=rot[i];
    }
}


void SetHomeVerse(PartInstance* p, int hv) {
    p->HomeVerse = hv;
}


void SetSize(PartInstance* p, vec3 s) {
    for (int i=0; i<3; i++) 
        p->Size[i]=s[i];
}


void SetPosition(PartInstance* p, vec3 pos) {
    mat4x4 temp;
    for (int i=0; i<3; i++) 
        p->Position[i]=pos[i];
    
    mat4x4_translate(temp, pos[0], pos[1], pos[2]);
    for (int r=0; r<3; r++) for (int c=0; c<3; c++)
        temp[r][c]=p->CFrame[r][c];
    mat4x4_dup(p->CFrame, temp);
}


void SetRotation(PartInstance* p, vec3 r) {
    mat4x4 temp;
    for (int i=0; i<3; i++) 
        p->Rotation[i]=r[i];

    mat4x4_identity(temp);
    // Euler YXZ
    mat4x4_rotate_Y(temp, temp, r[1]*TO_RAD);
    mat4x4_rotate_X(temp, temp, r[0]*TO_RAD);
    mat4x4_rotate_Z(temp, temp, r[2]*TO_RAD);
    for (int i=0; i<3; i++) 
        temp[3][i]=p->CFrame[3][i];
    mat4x4_dup(p->CFrame, temp);
}


void SetTranslation(PartInstance* p, vec3 size, vec3 position, vec3 rotation) {
    SetSize(p, size);
    SetPosition(p, position);
    SetRotation(p, rotation);
}


void SetMetaProperties(PartInstance* p, char* name, char* className, int homeVerse) {
    p->Name=name;
    p->ClassName=className;
    p->HomeVerse=homeVerse;
}


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


void RenderPartInstance(PartInstance* p) {

}