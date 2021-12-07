// Multi_verse DeliveryService
// Gavin Zimmerman

// Objects Source
#include "objects.h"


// SOURCE
// Drawing Functions
static void drawFromTriFan(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_FAN, 0, p->Vertices);
}
static void drawFromTriStrip(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, p->Vertices);
}
static void drawCubeFromTriStrip(PartInstance* p) {
    for (int i=0; i<6; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
}
static void drawUVSphereFromTriStrip(PartInstance* p) {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, p->Vertices);
}
static void drawCylinder(PartInstance* p) {
    int sides = (p->Triangles/4);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sides+2);
    glDrawArrays(GL_TRIANGLE_FAN, sides+2, sides+2);
    glDrawArrays(GL_TRIANGLE_STRIP, 2*(sides+2), 2*(sides+1));
}
static void drawCone(PartInstance* p) {
    int sides = p->Triangles / 3;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(sides+1));
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1), sides+2);
}
static void drawFrustumCone(PartInstance* p) {
    int sides = p->Triangles / 4;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(sides+1));
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1), sides+2);
    glDrawArrays(GL_TRIANGLE_FAN, 2*(sides+1) + sides+2, sides+2);
}
static void drawFromLineStrip(PartInstance* p) {
    glEnable(GL_LINE_SMOOTH);
    glDrawArrays(GL_LINE_STRIP, 0, p->Vertices);
}

// buildVAOVBO
    // @brief Helper function for setting up a VAO/VBO pair (unique per vertice set)
    // @param vao Address of VAO variable to set
    // @param vbo Address of VBO variable to set
    // @param verts Array of geometery data
    // @param size of geometery data array
static void buildVAOVBO(unsigned int* vao, unsigned int *vbo, float verts[], int n) {
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



// Part Functions
// Generic Constructor
    // @brief Creates a PartInstance object using malloc. 
    // All instance properties are set to their defaults.
    // @return Returns the created PartInstance
PartInstance* NewPartInstance() {
    PartInstance* p = (PartInstance*) malloc(sizeof(PartInstance));
    
    p->Name="";
    p->ClassName="";
    mat4x4_identity(p->CFrame);
    vec3_zero(p->Size);
    vec3_zero(p->Position);
    vec3_zero(p->Rotation);

    p->Vao = 0;
    p->Vbo = 0;
    p->Triangles =0;
    p->Vertices = 0;

    for (int i=0; i<3; i++) 
        p->Color[i]=255;
    p->Transparency=1;
    p->Shader = 0;

    return p;
}


// Buffer Deconstructor
    // @brief Will delete the VAO/VBO buffer/array assosiated with an object
    // @return Returns NULL
void* DeletePartInstanceBuffers(PartInstance* p) {
    if (glIsBuffer(p->Vbo))
        glDeleteBuffers(1, &p->Vbo);
    if (glIsVertexArray(p->Vao))
        glDeleteVertexArrays(1, &p->Vao);
    p->Vbo = 0;
    p->Vao = 0;

    return NULL;
}

// Generic Deconstructor 
    // @brief Will free Part Instance from memory. This should only be called once per part instance.
    // @return Returns NULL
void* DestroyPartInstance(PartInstance* p) {
    free(p);
    return NULL;
}


// Instance Constructors
// Line
    // @brief Contructs a part instance for rendering a line from 3D vector data
    // @param n Number of points in line
    // @pts Array of vec4 3d positions
    // @return Part Instance for drawing line
PartInstance* line(int n, vec4 pts[]) {
    // Build vertice data
    unsigned int vao, vbo;
    float verts[VERTEX_DATA_ROW * n];
    {
        for (int i=0; i<n; i++) {
            int off = i * VERTEX_DATA_ROW;
            vec4_set((float*) (verts+off), pts[i]);             // Position
            vec3_set((float*) (verts+off+4), (vec4){0,0,1});    // Normal
        }
    }
    buildVAOVBO(&vao, &vbo, verts, sizeof(verts));
    
    // Create Instance
    PartInstance* p = NewPartInstance();
    p->Draw = drawFromLineStrip;
    p->Vertices = n;
    p->Vao = vao;
    p->Vbo = vbo;
    vec3_set(p->Size, (vec3){1,1,1});
    p->ClassName = "Line";
    return p;
}

// Plane
    // @brief Contructs a part instance for rendering a plane
    // @param size x/z sizing of plane
    // @return Part Instance for drawing plane
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
    p->Draw = drawFromTriStrip;
    p->Vertices = 4;
    p->Triangles = 2;
    p->Vao = vao;
    p->Vbo = vbo;
    vec3_set(p->Size, (vec3){size,0,size});
    p->ClassName = "Plane";
    return p;
}

// Circle
    // @brief Contructs a part instance for rendering a circle
    // @param sides Number of sides to render on circle; inputs are standardized to reduce unique vaos/vbos
    // @param radius Radius of circle
    // @return Part Instance for drawing circle
PartInstance* circle(int sides, double radius) {
    static unsigned int vao[5] = {0}, vbo[5] = {0};
    unsigned int cvao = 0, cvbo = 0, *evao, *evbo;
    if (sides<=2)
        return NULL;
    
    // Set effective vao/ vbo int address to use
    int mem;
    switch(sides) {
        case INST_XLO_SEGMTS:   mem=0; break;   // Standard Params
        case INST_LOW_SEGMTS:   mem=1; break;
        case INST_MED_SEGMTS:   mem=2; break;
        case INST_HI_SEGMTS:    mem=3; break;
        case INST_XHI_SEGMTS:   mem=4; break;
        default:                mem=-1; break;  // Custom Params
    }
    if (mem==-1) {
        evao = &cvao; evbo = &cvbo;
    }
    else {
        evao = &vao[mem]; evbo = &vbo[mem];
    }
    
    // If not initialized or custom params, build vertice data from new
    if ((*evao==0) || (*evbo==0)) {
        float s = .5;
        float verts[VERTEX_DATA_ROW * (sides+2)];
        {
            // Triangle Fan
            int off=0;
            vec4_set((float*) verts, UNIT4_W);              // Position
            vec3_set((float*) (verts+4), UNIT3_Y);  // Normal
            for (int i=1; i<sides+2; i++) {
                double a = (float)i * 360.f / (float)sides; // Angle
                vec4 pos = {s*cosf(a*TO_RAD), 0.f, s*sinf(a*TO_RAD), 1.f};
                
                off = i * VERTEX_DATA_ROW;
                vec4_set((float*) (verts+off), pos);        // Set Position
                vec3_set((float*) (verts+off+4), UNIT3_Y);  // Set Normal
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->Draw = drawFromTriFan;
    p->Vertices = sides+2;
    p->Triangles = sides;
    p->Vao = *evao;
    p->Vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, 0, radius*2});
    p->ClassName = "Circle";
    return p;
}

// Cube
    // @brief Contructs a part instance for rendering a cube
    // @param size x/y/z sizing of cube
    // @return Part Instance for drawing cube
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
                    mat4x4_col(verts+off, tri, 3);  // Set Position
                    vec3_set(verts+off+4, tri[2]);  // Set Normal
                }
            }
        }
        buildVAOVBO(&vao, &vbo, verts, sizeof(verts));
    }

    // Create Instance
    PartInstance* p = NewPartInstance();
    p->Draw = drawCubeFromTriStrip;
    p->Vertices = 24;
    p->Triangles = 12;
    p->Vao = vao;
    p->Vbo = vbo;
    vec3_set(p->Size, (vec3){size, size, size});
    p->ClassName = "Cube";
    return p;
}



// SphereVertFromAngles
    // @brief Sets a vec4 to a position on a sphere of radius 1 given 2 angles
    // @param th Angle 1 (Horizontal XZ)
    // @param ph Angle 2 (Vertical Y)
static void SphereVertFromAngles(vec4 v, double th, double ph) {
    v[0] = cos(ph * TO_RAD) * sin(th * TO_RAD);
    v[1] = sin(ph * TO_RAD);
    v[2] = cos(ph * TO_RAD) * cos(th * TO_RAD);
    v[3] = 1.f;
}
// uvSphere
    // @brief Contructs a part instance for rendering a sphere built with quads (UV)
    // @param segments The amount of detail around the sphere (XZ). These inputs are standardized to reduce unqiue vbos/vaos
    // @param rings The amount of detail along the sphere (Y). These inputs are standardized to reduce unique vaos/vbos
    // @return Part Instance for drawing sphere. If not given enough segment/ring input, NULL is returned
    // since the requested shape is not qualified for drawing a sphere geometrically in 3D space.
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
    p->Draw = drawUVSphereFromTriStrip;
    p->Vertices = (rings+1) * (segments+1) * 2;
    p->Triangles = (rings+1) * segments * 2;
    p->Vao = *evao;
    p->Vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, radius*2, radius*2});
    p->ClassName = "UVSphere";
    return p;
}

// Cylinder
    // @brief Contructs a part instance for rendering a cylinder
    // @param sides Number of sides to render on cylinder; inputs are standardized to reduce unique vaos/vbos
    // @return Part Instance for drawing cylinder
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
    p->Draw = drawCylinder;
    p->Vertices = 2*(2*sides+3);
    p->Triangles = 4*sides;
    p->Vao = *evao;
    p->Vbo = *evbo;
    vec3_set(p->Size, (vec3){radius*2, depth, radius*2});
    p->ClassName = "Cylinder";
    return p;
}

// Cone
    // @brief Contructs a part instance for rendering a cone
    // @param sides Number of sides to render on cylinder; inputs are standardized to reduce unique vaos/vbos
    // @param rad0 Radius of bottom of cone
    // @param rad1 Radius of top of cone; If not 0, a new VAO/VBO will be created as the radius ratio is not standardized
    // @param depth Size of cone depth
    // @return Part Instance for drawing cone
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
                vec3 normal = {cosf(b)*cosf(a*TO_RAD), cosf(b), cosf(b)*sinf(a*TO_RAD)};

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
                    double a = 360-((float)i * (360.0/(float)sides)); // Angle
                    vec4 pos = {s*sinf(a*TO_RAD), sgn*s0, sgn* s*cosf(a*TO_RAD), 1};

                    vec4_set((float*) (verts+off), pos);        // Position
                    vec3_set((float*) (verts+off+4), normal);   // Normal
                }
            }
        }
        buildVAOVBO(evao, evbo, verts, sizeof(verts));
    }
    
    // Create Instance
    PartInstance* p = NewPartInstance();
    p->Draw = (frustum)? drawFrustumCone: drawCone;
    p->Vertices = (frustum?2:1)*(sides+2) + (2*(sides+1));
    p->Triangles = (frustum?4:3)*sides;
    p->Vao = *evao;
    p->Vbo = *evbo;
    vec3_set(p->Size, (vec3){rad0*2, depth, rad0*2});
    p->ClassName = (frustum)? "FrustumCone":"Cone";
    return p;
}


// clonePart
    // @brief Duplicates a part instance resuing all set properties including vao/vbo properties
    // @param src Part Instance to clone
    // @return Cloned Part Instance
PartInstance* clonePart(PartInstance* src) {
    PartInstance* p = NewPartInstance();
    memcpy(p, src, sizeof(PartInstance));
    return p;
}




// Part Instance Methods
void PartSetColor(PartInstance* p, color3 c) {
    for (int i=0; i<3; i++) 
        p->Color[i]=c[i];
}

void PartSetCFrame(PartInstance* p, mat4x4 cf) {
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

void PartSetSize(PartInstance* p, vec3 s) {
    for (int i=0; i<3; i++) 
        p->Size[i]=s[i];
}

void PartSetPosition(PartInstance* p, vec3 pos) {
    mat4x4 temp;
    for (int i=0; i<3; i++) 
        p->Position[i]=pos[i];
    
    mat4x4_translate(temp, pos[0], pos[1], pos[2]);
    for (int r=0; r<3; r++) for (int c=0; c<3; c++)
        temp[r][c]=p->CFrame[r][c];
    mat4x4_dup(p->CFrame, temp);
}

void PartSetRotation(PartInstance* p, vec3 r) {
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

void PartSetShader(PartInstance* p, GLuint s) {
    p->Shader=s;
}




// Jump Functions
// NewJumpInstance
    // @brief Creates a new Jump Instance, used for rendering an external verse object,
    // the camera is not currently hosted in; Uses malloc to allocate object memory
    // @return Allocated JumpInstance address.
JumpInstance* NewJumpInstance() {
    JumpInstance* j = (JumpInstance*) malloc(sizeof(JumpInstance));
    mat4x4_identity(j->CFrame0);
    mat4x4_identity(j->CFrame1);
    j->ClassName="Jump";
    j->Verse0=NULL;
    j->Verse1=NULL;
    j->Radius=5;
    return j;
}

// DestroyJumpInstance
    // @brief Deallocates memory for a JumpInstance object
    // @param j The jump instance to free up
void *DestroyJumpInstance(JumpInstance* j) {
    free(j);
    return NULL;
}

// JumpConnectVerses
    // @brief Will bind to verses together with a JumpInstance; 
    // ordering of verses should follow same ordering of JumpSetCFrames
    // @param j Jump Instance to bind verses on
    // @pram v0 First verse to connect
    // @param v1 Second verse to connect
void JumpConnectVerses(JumpInstance* j, VerseInstance* v0, VerseInstance* v1) {
    if (j->Verse0) VerseRemoveJump(v0, j);
    if (j->Verse1) VerseRemoveJump(v1, j);
    VerseAddJump(v0, j);
    VerseAddJump(v1, j);
    j->Verse0=v0;
    j->Verse1=v1;
}

// JumpConnectVerses
    // @brief Will bind to verses together with a JumpInstance; 
    // ordering of verses should follow same ordering of JumpConnectVerses
    // @param j Jump Instance to bind verses on
    // @pram CF0 CFrame location in j.Verse0 to render v.Verse1 at; Portal drawn around Z axis
    // @param CF1 CFrame location in j.Verse1 to render v.Verse0 at; Portal drawn around Z axis
void JumpSetCFrames(JumpInstance* j, mat4x4 CF0, mat4x4 CF1) {
    mat4x4_dup(j->CFrame0, CF0);
    mat4x4_dup(j->CFrame1, CF1);

    // Calculate a matrix to translate CFrames between verses
    // Repeat for both sides
    for (int k=0; k<2; k++) {
        mat4x4 ToB, M;
        
        // First apply source CFrame matrix
        // This exists in the current
        mat4x4_dup(ToB, (!k)? CF0 : CF1); // Source
        
        // Rotate by 180 degrees (in place)
        // This is because portals always are drawn in the same direction, but are opposite ends in translated space
        mat4x4_identity(M);
        mat4x4_rotate_Y(M,M,180*TO_RAD);
        mat4x4_mul(ToB, ToB, M);

        // Apply inverse of destination CFrame
        // Similar to Camera, we want the offset of the jump to the desination verses's origin
        mat4x4_invert(M, (!k)? CF1 : CF0);
        mat4x4_mul(ToB, ToB, M);

        // Set appropriate property
        mat4x4_dup((!k)? j->_ToV1 : j->_ToV0, ToB); // Output Translation
    }
}




// VerseAddPart
    // @brief Adds a part as a 'child' under a VerseInstance
    // @param v Verse Instance to add part to
    // @param p Part Instance to be added
    // @return 0 on success; -1 if part could not be added
int VerseAddPart(VerseInstance* v, PartInstance* p) {
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        if (v->Children[i]==NULL) {
            v->Children[i] = p;
            return 0;
        }
        else if (v->Children[i]==p) {
            return 0;
        }
    }
#ifdef DEVMODE
    printf("Could not add part (%s) to verse (%s)\n", p->Name, v->Name);
#endif
    return -1;
}

// VerseAddJump
    // @brief Adds a jump under a VerseInstance
    // @param v Verse Instance to add part to
    // @param j Jump Instance to be added
    // @return 0 on success; -1 if part could not be added
int VerseAddJump(VerseInstance* v, JumpInstance* j) {
    for (int i=0; i<MAX_JUMP_INSTANCES; i++) {
        if (v->Jumps[i]==NULL) {
            v->Jumps[i] = j;
            return 0;
        }
        else if (v->Jumps[i]==j) {
            return 0;
        }
    }
#ifdef DEVMODE
    printf("Could not add jump (%s) to verse (%s)\n", j->Name, v->Name);
#endif
    return -1;
}

// VerseRemovePart
    // @brief Removes a part under a VerseInstance; Does not free part from memory
    // @param v Verse Instance to remove part from
    // @param p Part Instance to be removed
    // @return 0 on success; -1 if part could not be removed/found
int VerseRemovePart(VerseInstance* v, PartInstance* p) {
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        if (v->Children[i]==p) {
            v->Children[i] = NULL;
            for (int j=i; j<MAX_PART_INSTANCES-1; j++) {
                v->Children[j]=v->Children[j+1];
            }
            v->Children[MAX_PART_INSTANCES-1]=NULL;
            return 0;
        }
    }
    return -1;
}

// VerseRemoveJump
    // @brief Removes a jump under a VerseInstance; Does not free jump from memory
    // @param v Verse Instance to remove jump from
    // @param p Jump Instance to be removed
    // @return 0 on success; -1 if jump could not be removed/found
int VerseRemoveJump(VerseInstance* v, JumpInstance* j) {
    for (int i=0; i<MAX_JUMP_INSTANCES; i++) {
        if (v->Jumps[i]==j) {
            v->Jumps[i] = NULL;
            for (int j=i; j<MAX_JUMP_INSTANCES-1; j++) {
                v->Jumps[j]=v->Jumps[j+1];
            }
            v->Jumps[MAX_JUMP_INSTANCES-1]=NULL;
            return 0;
        }
    }
    return -1;
}


// VerseFindFirstPart
    // @brief Will return the first PartInstance parented under a VerseInstance with a given Name
    // @param v Verse Instance to search
    // @param Name Name to search for
    // @return If a Part Instance is found with the corresponding name, it is returned;
    // Otherwise NULL is returned
PartInstance* VerseFindFirstPart(VerseInstance* v, char* Name) {
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        if ( (v->Children[i]!=NULL) && (strcmp(v->Children[i]->Name, Name)==0) )
            return v->Children[i];
    }
    return NULL;
}

// VerseFindFirstJump
    // @brief Will return the first JumpInstance parented under a VerseInstance with a given Name
    // @param v Jump Instance to search
    // @param Name Name to search for
    // @return If a Jump Instance is found with the corresponding name, it is returned;
    // Otherwise NULL is returned
JumpInstance* VerseFindFirstJump(VerseInstance* v, char* Name) {
    for (int i=0; i<MAX_JUMP_INSTANCES; i++) {
        if ( (v->Jumps[i]!=NULL) && (strcmp(v->Jumps[i]->Name, Name)==0) )
            return v->Jumps[i];
    }
    return NULL;
}





// Shaders
// Credit to Willem A. (Vlakkies) Schreuder
    // @brief Will print log of information about built shader; used for debugging
static void printShaderLog(GLuint shader, char* filename) {
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

// BuildShader
    // @brief Will compiler a shader from a given file's source text;
    // prints any errors in compiling individual shaders
    // @param type The type of shader to create
    // @param filename The name of the file to read
    // @throws Will throw a fatal error if shader file cannot be read correctly
    // @return Returns 
GLuint BuildShaderFromFile(GLenum type, char* filename) {
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
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**) &source, NULL);
    glCompileShader(shader);
    free(source);


    printShaderLog(shader, filename);
    return shader;
}
