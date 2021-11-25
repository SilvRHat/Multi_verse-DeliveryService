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
    static int init = 0;
    static unsigned int vaoId = 0, vboId = 0;

    // Setup Part
    PartInstance* inst = empty();
    inst->ClassName="Cube";
    for (int i=0; i<3; i++)
        inst->Size[i]=size;

    // Generate vertices
    if (!init) {
        init = 1;
        // Generate cube
        float cube_verts[VERTEX_DATA_ROW * 36];
        {
            float s = .5;
            mat4x4 front_verts[6];
            mat4x4_translate(front_verts[0], s, s, s); mat4x4_translate(front_verts[1],-s, s, s); mat4x4_translate(front_verts[2], s,-s, s);
            mat4x4_translate(front_verts[3],-s, s, s); mat4x4_translate(front_verts[4], s,-s, s); mat4x4_translate(front_verts[5],-s,-s, s);
            // Rotate around sides
            for (int i=0; i<6; i++) {
                mat4x4 rot, tri;
                mat4x4_identity(rot);
                if (i<4) // Front, Back, Left, Right
                    mat4x4_rotate_X(rot, rot, 90*i*TO_RAD);
                else // Top & Bottom
                    mat4x4_rotate_Y(rot, rot, 90*TO_RAD * (i==5?-1:1));
                for (int v=0; v<6; v++) {
                    int off = ((i*6)+v) * VERTEX_DATA_ROW;
                    mat4x4_mul(tri, rot, front_verts[v]);
                    mat4x4_col(cube_verts+off, tri, 3);         // Vertex Position Data
                    cube_verts[off+4] = tri[2][0];              // Normal Data
                    cube_verts[off+5] = tri[2][1];
                    cube_verts[off+6] = tri[2][2];
                    /*printf("{%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f\t%.0f}\n", 
                        cube_verts[off+0],
                        cube_verts[off+1],
                        cube_verts[off+2],
                        cube_verts[off+3],
                        cube_verts[off+4],
                        cube_verts[off+5],
                        cube_verts[off+6]);*/
                }
            }
        }
        // Credit to Willem A. (Vlakkies) Schreuder for code structure binding VAO and VBO
        // Sourced from ex27.c [lines 432]

        glGenVertexArrays(1,&vaoId);
        glBindVertexArray(vaoId);

        //  Get buffer name
        glGenBuffers(1,&vboId);
        //  Bind VBO
        glBindBuffer(GL_ARRAY_BUFFER,vboId);
        //  Copy cube data to VBO
        glBufferData(GL_ARRAY_BUFFER,sizeof(cube_verts),cube_verts,GL_STATIC_DRAW);

        //  Release VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
    inst->vertices = 36;
    inst->vao = vaoId;
    inst->vbo = vboId;

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
    p->shaderId = DEFAULT_SHADER;
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



int VerseAddChild(VerseInstance* v, PartInstance* p) {
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
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
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
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
    fread(source,n,sizeof(char),fp); source[n]='\x00';
    fclose(fp); 

    // Build Shader
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**) &source, NULL);
    glCompileShader(shader);
    free(source);

    printShaderLog(shader, filename);
    return shader;
}