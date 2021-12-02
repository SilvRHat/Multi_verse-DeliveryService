// Multi_verse DeliveryService
// Gavin Zimmerman

// Objects Source
#include "objects.h"


// SOURCE

// Drawing Functions
static void drawFromTriFan(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_FAN, 0, p->vertices);
}
static void drawFromTriStrip(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, p->vertices);
}
static void drawCubeFromTriStrip(PartInstance* p) {
    for (int i=0; i<6; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
}
static void drawUVSphereFromTriStrip(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, p->vertices);
}
static void drawCylinder(PartInstance* p) {
    int sides = (p->triangles/4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sides+2);
    glDrawArrays(GL_TRIANGLE_FAN, sides+2, sides+2);
    glDrawArrays(GL_TRIANGLE_STRIP, 2*(sides+2), 2*(sides+1));
}
static void drawCone(PartInstance* p) {
    int sides = p->triangles / 3;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(sides+1));
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1), sides+2);
}
static void drawFrustumCone(PartInstance* p) {
    int sides = p->triangles / 4;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(sides+1));
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1), sides+2);
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1) + sides+2, sides+2);
}
static void drawFromLineStrip(PartInstance* p) {
    GLint range[2];
    glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, range);
    glEnable(GL_LINE_SMOOTH);
    glDrawArrays(GL_LINE_STRIP, 0, p->vertices);
}



// Generic Constructor
PartInstance* NewPartInstance() {
    PartInstance* p = (PartInstance*) malloc(sizeof(PartInstance));
    
    p->Name="";
    p->ClassName="";
    mat4x4_identity(p->CFrame);
    p->HomeVerse=0;
    vec3_zero(p->Size);
    vec3_zero(p->Position);
    vec3_zero(p->Rotation);
    for (int i=0; i<3; i++) 
        p->Color[i]=255;

    SetShader(p, 0);
    p->vao = 0;
    p->vbo = 0;

    return p;
}

// Buffer Deconstructor
void* DeletePartInstanceBuffers(PartInstance* p) {
    if (glIsBuffer(p->vbo))
        glDeleteBuffers(1, &p->vbo);
    if (glIsVertexArray(p->vao))
        glDeleteVertexArrays(1, &p->vao);
    p->vbo = 0;
    p->vao = 0;

    return NULL;
}

// Generic Deconstructor 
void* DestroyPartInstance(PartInstance* p) {
    free(p);
    return NULL;
}

#define ATTRIB_POS_VERTEX 1
#define ATTRIB_POS_NORMAL 2
void buildVAOVBO(unsigned int* vao, unsigned int *vbo, float verts[], int n) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, n, verts, GL_STATIC_DRAW);

    glVertexAttribPointer(ATTRIB_POS_VERTEX, 4, GL_FLOAT, GL_FALSE, VERTEX_DATA_ROW*sizeof(GL_FLOAT), (void*) 0);
    glEnableVertexAttribArray(ATTRIB_POS_VERTEX);
    glVertexAttribPointer(ATTRIB_POS_NORMAL, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_ROW*sizeof(GL_FLOAT), (void*) 4);
    glEnableVertexAttribArray(ATTRIB_POS_NORMAL);
}


// Instance Constructors
PartInstance* line(int n, vec4 pts[]) {
    // Build vertice data
    unsigned int vao, vbo;
    float verts[VERTEX_DATA_ROW * n];
    {
        for (int i=0; i<n; i++) {
            int off = i * VERTEX_DATA_ROW;
            vec4_set((float*) (verts+off), pts[i]);             // Position
            vec3_set((float*) (verts+off+3), (vec4){0,0,1});    // Normal
        }
    }
    buildVAOVBO(&vao, &vbo, verts, sizeof(verts));

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawFromLineStrip;
    p->vertices = n;
    p->vao = vao;
    p->vbo = vbo;
    vec3_set(p->Size, (vec3){1,1,1});
    return p;
}


PartInstance* plane(double size) {
    static unsigned int vao = 0, vbo = 0;
    static int init = 0;

    if (!init) {
        init=1;
        float s=.5;
        float verts[] = {
             s, 0, s, 1, 0, 1, 0,
            -s, 0, s, 1, 0, 1, 0,
             s, 0,-s, 1, 0, 1, 0,
            -s, 0,-s, 1, 0, 1, 0,
        };
        buildVAOVBO(&vao, &vbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawFromTriStrip;
    p->vertices = 4;
    p->triangles = 2;
    p->vao = vao;
    p->vbo = vbo;
    vec3_set(p->Size, (vec3){size,0,size});
    p->ClassName = "Plane";
    return p;
}


PartInstance* circle(int sides, double radius) {
    static unsigned int vao[5] = {0}, vbo[5] = {0};
    unsigned int cvao = 0, cvbo = 0, *evao, *evbo;
    if (sides<=2)
        return NULL;
    
    // Set effective vao/ vbo integer to use
    int mem = -1;
    switch(sides) {
        case INST_XLO_SEGMTS:   mem=0; break;
        case INST_LOW_SEGMTS:   mem=1; break;
        case INST_MED_SEGMTS:   mem=2; break;
        case INST_HI_SEGMTS:    mem=3; break;
        case INST_XHI_SEGMTS:   mem=4; break;
        default:                mem=-1; break;
    }
    if (mem==-1) {
        evao = &cvao; evbo = &cvbo;
    }
    else {
        evao = &vao[mem]; evbo = &vbo[mem];
    }
    
    // If not initialized or custom params, build vertice
    if ((*evao==0) || (*evbo==0)) {
        float s = .5;
        float verts[VERTEX_DATA_ROW * (sides+2)];
        {
            // Triangle Fan
            int off=0;
            vec4_zero((float*) verts);              // Position
            vec3_set((float*) (verts+4), UNIT3_Y);  // Normal
            for (int i=1; i<sides+2; i++) {
                double a = (float)i * 360.f / (float)sides; // Angle
                vec4 pos = {s*cosf(a*TO_RAD), 0.f, s*sinf(a*TO_RAD), 1.f};
                
                off = i * VERTEX_DATA_ROW;
                vec4_set((float*) (verts+off), pos);        // Position
                vec3_set((float*) (verts+off+4), UNIT3_Y);  // Normal
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawFromTriFan;
    p->vertices = sides+2;
    p->triangles = sides;
    p->vao = *evao;
    p->vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, 0, radius*2});
    p->ClassName = "Circle";
    return p;
}


PartInstance* cube(double size) {
    static unsigned int vao = 0, vbo = 0;
    static int init = 0;

    // Generate vertices
    if (!init) {
        init = 1;
        // Generate cube
        float verts[VERTEX_DATA_ROW * 24];
        {
            float s = .5;
            mat4x4 front_verts[4];
            mat4x4_translate(front_verts[0], s, s, s); mat4x4_translate(front_verts[1], s,-s, s);
            mat4x4_translate(front_verts[2],-s, s, s); mat4x4_translate(front_verts[3],-s,-s, s);
            // Rotate around sides
            for (int i=0; i<6; i++) {
                mat4x4 rot, tri;
                mat4x4_identity(rot);
                if (i<4) // Front, Back, Left, Right Rotations
                    mat4x4_rotate_X(rot, rot, 90*i*TO_RAD);
                else // Top & Bottom
                    mat4x4_rotate_Y(rot, rot, 90*TO_RAD * (i==5?-1:1));
                for (int v=0; v<4; v++) {
                    int off = ((i*4)+v) * VERTEX_DATA_ROW;
                    mat4x4_mul(tri, rot, front_verts[v]); // Rotate front face
                    mat4x4_col(verts+off, tri, 3);  // Vertex Position Data
                    vec3_set(verts+off+4, tri[2]);  // Normal Data
                }
            }
        }
        buildVAOVBO(&vao, &vbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawCubeFromTriStrip;
    p->vertices = 24;
    p->triangles = 12;
    p->vao = vao;
    p->vbo = vbo;
    vec3_set(p->Size, (vec3){size, size, size});
    p->ClassName = "Cube";
    return p;
}




static void SphereVertFromAngles(vec4 v, double th, double ph) {
    v[0] = cos(ph * TO_RAD) * sin(th * TO_RAD);
    v[1] = sin(ph * TO_RAD);
    v[2] = cos(ph * TO_RAD) * cos(th * TO_RAD);
    v[3] = 1.f;
}

PartInstance* uvSphere(int segments, int rings, double radius) {
    static unsigned int vao[5][5] = {{0,0,0,0,0}}, vbo[5][5] = {{0,0,0,0,0}};
    unsigned int cvao = 0, cvbo = 0, *evao, *evbo;
    if ((segments<=2) || (rings<=1))
        return NULL;
    
    // Set effective vao/ vbo integer to use
    int mem_sg=0, mem_rg=0;
    switch(segments) {
        case INST_XLO_SEGMTS:   mem_sg=0; break;
        case INST_LOW_SEGMTS:   mem_sg=1; break;
        case INST_MED_SEGMTS:   mem_sg=2; break;
        case INST_HI_SEGMTS:    mem_sg=3; break;
        case INST_XHI_SEGMTS:   mem_sg=4; break;
        default:                mem_sg=-1; break;
    }
    switch(rings) {
        case INST_XLO_RINGS:    mem_rg=0; break;
        case INST_LOW_RINGS:    mem_rg=1; break;
        case INST_MED_RINGS:    mem_rg=2; break;
        case INST_HI_RINGS:     mem_rg=3; break;
        case INST_XHI_RINGS:    mem_rg=4; break;
        default:                mem_rg=-1; break;
    }
    if ((mem_rg==-1) || (mem_sg==-1)) {
        evao=&cvao; evbo=&cvbo;
    }
    else {
        evao=&(vao[mem_sg][mem_rg]); evbo=&(vbo[mem_sg][mem_rg]);
    }


    if ((*evao==0) || (*evbo==0)) {
        float verts[VERTEX_DATA_ROW * (rings+1) * (segments+1) * 2];
        {
            double ph_inc = 180.0/(rings+1), th_inc = 360.0/segments;
            for (int r=0; r<rings+1; r++) {
                for (int s=0; s<segments+1; s++) {
                    double ph = fmod(ph_inc*r, 180.0) - 90.0;
                    double th = fmod(th_inc*s, 360.0);
                    int off = (r*(segments+1) + s) * 2 * VERTEX_DATA_ROW;

                    // Get Vert Positions
                    vec4 p1, p2;
                    SphereVertFromAngles(p1, th, ph); vec3_scale(p1, p1, .5);
                    SphereVertFromAngles(p2, th, ph+ph_inc); vec3_scale(p2, p2, .5);
                    
                    // Set Positions
                    vec4_set(verts+off, p1);    // Vert1 Position
                    vec3_set(verts+off+4, p1);  // Vert1 Normal
                    vec4_set(verts+off + VERTEX_DATA_ROW, p2);  // Vert2 Position
                    vec3_set(verts+off+4+VERTEX_DATA_ROW, p2);  // Vert2 Normal
                }
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawUVSphereFromTriStrip;
    p->vertices = (rings+1) * (segments+1) * 2;
    p->triangles = segments * (rings+1) * 2;
    p->vao = *evao;
    p->vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, radius*2, radius*2});
    p->ClassName = "UVSphere";
    return p;
}


PartInstance* cylinder(int sides, double radius, double depth) {
    static unsigned int vao[5] = {0}, vbo[5] = {0};
    unsigned int cvao = 0, cvbo = 0, *evao, *evbo;
    if (sides<=2)
        return NULL;
    
    // Set effective vao/ vbo integer to use
    int mem = -1;
    switch(sides) {
        case INST_XLO_SEGMTS:   mem=0; break;
        case INST_LOW_SEGMTS:   mem=1; break;
        case INST_MED_SEGMTS:   mem=2; break;
        case INST_HI_SEGMTS:    mem=3; break;
        case INST_XHI_SEGMTS:   mem=4; break;
        default:                mem=-1; break;
    }
    if (mem==-1) {
        evao = &cvao; evbo = &cvbo;
    }
    else {
        evao = &vao[mem]; evbo = &vbo[mem];
    }
    

    if ((*evao==0) || (*evbo==0)) {
        float verts[2*(2*sides+3) * VERTEX_DATA_ROW];
        {
            // Triangle Fans (Top & Bottom) [Each (sides+2) vertices]
            for (int bt=0; bt<2; bt++) {
                float s = .5; float sgn = (bt?-1.0:1.0);
                
                int off0 = bt*(sides+2) * VERTEX_DATA_ROW;
                vec3 normal = {0, sgn, 0};
                vec4_set((float*) (verts+off0), (vec4){0,s,0,1});
                vec3_set((float*) (verts+off0+4), normal);
                for (int i=0; i<sides+1; i++) {
                    int off = (i+1) * VERTEX_DATA_ROW + off0;
                    double a = (float)i * (360.0/(float)sides); // Angle
                    vec4 pos = {s*cosf(a*TO_RAD), sgn*s, sgn*s*sinf(a*TO_RAD), 1};

                    vec4_set((float*) (verts+off), pos);        // Position
                    vec3_set((float*) (verts+off+4), normal);   // Normal
                }
            }

            // Triangle Strip (Side) [2(sides+1) vertices]
            int off1 = 2*(sides+2) * VERTEX_DATA_ROW;
            for (int i=0; i<sides+1; i++) {
                float s=.5;
                int off = (i*2)*VERTEX_DATA_ROW + off1;
                double a = (float)i * (360.0/(float)sides); // Angle
                vec4 p1 = {s*cosf(a*TO_RAD), s, s*sinf(a*TO_RAD), 1};
                vec4 p2 = {s*cosf(a*TO_RAD), -s, s*sinf(a*TO_RAD), 1};
                vec3 normal = {cosf(a*TO_RAD), 0, sinf(a*TO_RAD)};

                vec4_set((float*) (verts+off), p1);         // Vert1 Position
                vec3_set((float*) (verts+off+4), normal);   // Vert1 Normal
                vec4_set((float*) (verts+off+VERTEX_DATA_ROW), p2);         // Vert2 Position
                vec3_set((float*) (verts+off+4+VERTEX_DATA_ROW), normal);   // Vert2 Normal
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = drawCylinder;
    p->vertices = 2*(2*sides+3);
    p->triangles = 4*sides;
    p->vao = *evao;
    p->vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, depth, radius*2});
    p->ClassName = "Cylinder";
    return p;
}


PartInstance* cone(int sides, double rad0, double rad1, double depth) {
    static unsigned int vao[5], vbo[5];
    unsigned int cvao = 0, cvbo = 0, *evao, *evbo;
    int frustum = (rad1!=0);

    // Set effective vao/ vbo integer to use
    int mem = -1;
    switch(sides) {
        case INST_XLO_SEGMTS:   mem=0; break;
        case INST_LOW_SEGMTS:   mem=1; break;
        case INST_MED_SEGMTS:   mem=2; break;
        case INST_HI_SEGMTS:    mem=3; break;
        case INST_XHI_SEGMTS:   mem=4; break;
        default:                mem=-1; break;
    }
    if ((mem==-1) || (frustum)) {
        evao = &cvao; evbo = &cvbo;
    }
    else {
        evao = &vao[mem]; evbo = &vbo[mem];
    }

    if ((*evao==0) || (*evbo==0)) {
        float verts[ ((frustum?2:1)*(sides+2) + (2*(sides+1))) * VERTEX_DATA_ROW];
        {
            // Modified cylinder code
            // Triangle Strip (Side) [2(sides+1) vertices]
            float s0 = .5, s1 = (rad0>0)? .5*rad1/rad0 : 1;
            for (int i=0; i<sides+1; i++) {
                int off = (i*2)*VERTEX_DATA_ROW;
                
                // Calculate Vertices
                double a = (float)i * (360.0/(float)sides); // Angle
                vec4 p1 = {s0*sinf(a*TO_RAD), -s0, s0*cosf(a*TO_RAD), 1};
                vec4 p2 = {s1*sinf(a*TO_RAD), s0, s1*cosf(a*TO_RAD), 1};
                
                // Calculate Normal
                float sl = 1/(rad0-rad1); 
                double b = atanf(sl);
                vec3 normal = {cosf(b)*sinf(a*TO_RAD), cosf(b), cosf(b)*cosf(a*TO_RAD)};

                vec4_set((float*) (verts+off), p1);         // Vert1 Position
                vec3_set((float*) (verts+off+4), normal);   // Vert1 Normal
                vec4_set((float*) (verts+off+VERTEX_DATA_ROW), p2);         // Vert2 Position
                vec3_set((float*) (verts+off+4+VERTEX_DATA_ROW), normal);   // Vert2 Normal
            }

            // Triangle Fans (Bottom & Top (if frustum)) [Each (sides+2) vertices]
            int off0 = 2*(sides+1) * VERTEX_DATA_ROW;
            for (int bt=0; bt<(frustum?2:1); bt++) {
                float sgn = (bt?1:-1);
                float s = (bt?s1:s0);
                
                int off1 = bt*(sides+2) * VERTEX_DATA_ROW + off0;
                vec3 normal = {0, sgn, 0};
                vec4_set((float*) (verts+off1), (vec4){0,sgn*s0,0,1});
                vec3_set((float*) (verts+off1+4), normal);
                for (int i=0; i<sides+1; i++) {
                    int off = (i+1) * VERTEX_DATA_ROW + off1;
                    double a = (float)i * (360.0/(float)sides); // Angle
                    vec4 pos = {s*cosf(a*TO_RAD), sgn*s0, sgn * s*sinf(a*TO_RAD), 1};

                    vec4_set((float*) (verts+off), pos);        // Position
                    vec3_set((float*) (verts+off+4), normal);   // Normal
                }
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }
    
    // Create Instance
    PartInstance* p = NewPartInstance();
    p->_drawFunc = (frustum)? drawFrustumCone: drawCone;
    p->vertices = (frustum?2:1)*(sides+2) + (2*(sides+1));
    p->triangles = (frustum?4:3)*sides;
    p->vao = *evao;
    p->vbo = *evbo;
    vec3_set(p->Size, (vec3){rad0*2, depth, rad0*2});
    p->ClassName = "Cone";
    return p;
}


PartInstance* clonePart(PartInstance* src) {
    // Create Instance
    PartInstance* p = NewPartInstance();
    memcpy(p, src, sizeof(PartInstance));
    return p;
}




// Part Instance Methods
void SetColor(PartInstance* p, color3 c) {
    for (int i=0; i<4; i++) 
        p->Color[i]=c[i];
    p->Color[3]=255;
}


void SetCFrame(PartInstance* p, mat4x4 cf) {
    vec4 pos, rot;
    //float rx, ry, rz;

    // TODO
    // Find YXZ Euler Angles from Rotation Matrix
    // Credit: http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
    /*if (1 - fabsf(cf[0][2]) < 1e-3) {

    }
    else { 
        // Gimbal lock edge case

    */

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


void SetShader(PartInstance* p, unsigned int s) {
    p->shader =s;
}


JumpInstance* NewJump() {
    JumpInstance* j = (JumpInstance*) malloc(sizeof(JumpInstance));
    mat4x4_identity(j->CFrame0);
    mat4x4_identity(j->CFrame1);
    j->ClassName="Jump";
    j->Verse0=NULL;
    j->Verse1=NULL;
    j->Radius=5;
}

void *DestroyJump(VerseInstance* j) {
    free(j);
    return NULL;
}

void JumpConnectVerses(VerseInstance* v0, VerseInstance* v1) {

}

void JumpSetCFrames(mat4x4 CF0, mat4x4 CF1) {

}



int VerseAddChild(VerseInstance* v, PartInstance* p) {
    for (int i=0; i<MAX_INSTANCES; i++) {
        if (v->Children[i]==NULL) {
            v->Children[i] = p;
            return 0;
        }
    }
#ifdef DEVMODE
    printf("Could not add child (%s) to verse (%s)\n", p->Name, v->Name);
#endif
    return -1;
}


PartInstance* VerseFindFirstChild(VerseInstance* v, char* Name) {
    for (int i=0; i<MAX_INSTANCES; i++) {
        if ( (v->Children[i]!=NULL) && (strcmp(v->Children[i]->Name, Name)==0) )
            return v->Children[i];
    }
    return NULL;
}



// Shaders
// Credit to Willem A. (Vlakkies) Schreuder
void printShaderLog(int shader, char* filename) {
    int len=0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len>1) {
        int n=0;
        char* buffer = (char*)malloc(len);
        if (!buffer) Error("Cannot allocate %d bytes fir shader log\n", len);
        glGetShaderInfoLog(shader, len, &n, buffer);
        fprintf(stderr, "%s:\n%s\n", filename, buffer);
    }   
    glGetShaderiv(shader, GL_COMPILE_STATUS, &len);
    if (!len) Error("Error compiling %s\n",filename);
}


int BuildShader(GLenum type, char* filename) {
    // Open source file and get size
    FILE* fp = fopen(filename, "r");
    if (!fp) Error("Cannot open file: %s\n", filename);
    fseek(fp,0,SEEK_END);
    int n = ftell(fp);
    rewind(fp);

    // Allocate and read source
    char* source = malloc(n+1);
    if (!source) Error("Could not allocate space for shader (%s) source\n", filename);
    if (fread(source,sizeof(char),n,fp)!=n) Error("Could not read shader (%s) source\n", filename);
    source[n]='\x00';
    fclose(fp); 

    // Build Shader
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**) &source, NULL);
    glCompileShader(shader);
    free(source);

    printShaderLog(shader, filename);
    return shader;
}

