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
    if (1 - fabsf(cf[0][2]) < 1e-3) {

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


int VerseAddChild(VerseInstance v, PartInstance* p) {
    printf("Adding to [%s]\n", v.Name);
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        if (v.Children[i]!=NULL)
            continue;
        printf("Child is [%p][%d]\n", v.Children[i], i);
        v.Children[i] = p;
        printf("Child is now [%p][%d]\n", v.Children[i], i);
        printf("I make cube at [%d]\n", i);
        printf("Cube is [%p]\n", p);
        return 0;
    }
#ifdef DEVMODE
    printf("Could not add child (%s) to verse (%s)\n", p->Name, v.Name);
#endif
    return -1;
}


PartInstance* VerseFindFirstChild(VerseInstance v, char* Name) {
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        if ( (v.Children[i]!=NULL) && (strcmp(v.Children[i]->Name, Name)==0) )
            return v.Children[i];
    }
    return NULL;
}