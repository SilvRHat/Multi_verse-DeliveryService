// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#7007 - Gate Source
#include "simple.h"

// Shaders

// VERSE

static void buildVerse(GLFWwindow* window);
static void cleanVerse(GLFWwindow* window);


VerseInstance SIMPLE_VERSE = {
    .Name = "Simple",
    .ClassName = "Verse7007",

    .Build = buildVerse,
    .Clean = cleanVerse,

    .Jumps = {NULL},
    .Root = {
        .Name = "Root",
        .ClassName = "RootOrigin",
        .Parent = NULL,
        .CanRender = GL_TRUE,
        ._Malloced = GL_FALSE,

        .Parts = {NULL},
        .Lights = {NULL},
        .Origins = {NULL}
    }
};

static VerseInstance* self;
static OriginInstance* root;



// Signal Functions
static void ResizeProjection(va_list args) {
    int width, height; {
        va_arg(args, GLFWwindow*); // window
        width = va_arg(args, int);
        height = va_arg(args, int);
    }
    
    mat4x4_perspective(self->ProjectionMatrix, 60*TO_RAD, 
        (float)width/height, NEAR_PROJECTION, FAR_PROJECTION);
    
}

static void updateLight() {
    OriginInstance* o = OriginFindFirstOrigin(root, "LightOri", 0);

    double t = glfwGetTime();
    double r = 10;
    double s = 3.f/4.f;
    mat4x4 pos;
    mat4x4_from_pos(pos, (vec3) {r*cos(t*s), 4, r*sin(t*s)});
    if (o) {
        OriginSetCFrame(o, pos);
    }
}

static void buildVerse(GLFWwindow* window) {
    PartInstance* nwp;
    LightInstance* nwl;
    self = &SIMPLE_VERSE;
    root = &(self->Root);
    mat4x4_identity(self->Root.CFrame);

    // Init Verse
    {
        int width, height;
        SignalConnect(&FrameBufferSize, ResizeProjection);
        glfwGetFramebufferSize(window, &width, &height);
        mat4x4_perspective(self->ProjectionMatrix, 60*TO_RAD, 
            (float)width/height, NEAR_PROJECTION, FAR_PROJECTION);
    }


    // Build scene    
    // Axis
    vec4 axis_vert[6] = {
        {0,0,0,1}, {2,0,0,1},
        {0,0,0,1}, {0,2,0,1}, 
        {0,0,0,1}, {0,0,2,1}
    };
    vec3 axis_norm[2] = {{0}};
    vec2 axis_uv[2] = {{0}};
    for (int i=0; i<3; i++) {
        nwp = primitive(2, axis_vert+(i*2), axis_norm, axis_uv, GL_LINES); {
            color4 c = {0,0,0,255};
            c[i]=255;
            PartSetColor(nwp, c);
            PartSetParent(nwp, root);
        }
    }

    OriginInstance* lightOri = NewOriginInstance(); {
        lightOri->Name="LightOri";
        OriginSetParent(lightOri, root);
    }
    
    nwp = canvas(60); {
        PartSetRotation(nwp, (vec3){-90*TO_RAD,0,0}, EULER_XYZ);
        PartSetParent(nwp, root);
        PartSetPosition(nwp, (vec3){0,-2,0});
        PartSetColor(nwp, (color4){0,0,0,0});
    }
    
    nwp = cube(3); {
        PartSetRotation(nwp, (vec3){45*TO_RAD,45*TO_RAD,0}, EULER_XYZ);
        PartSetPosition(nwp, (vec3){10,1e-3,0});
        PartSetColor(nwp, (color4){0xff, 0x00, 0xff, 0xff});
        PartSetParent(nwp, root);
    }
    
    nwl = ambientLight(); {
        LightSetColor(nwl, (color4) {0x11, 0x11, 0x11, 0xff});
        LightSetParent(nwl, lightOri);
        nwl->LightGroup = GLOBAL_LIGHTGROUP;
        nwl->BlendSrc = GL_ONE;
        nwl->BlendDst = GL_ZERO;
        nwl->Name = "Ambient";
    }

    mat4x4 m, n;
    mat4x4_from_pos(m, (vec3){0,0,-10});
    mat4x4_from_pos(n, (vec3){0,0,10});

    JumpInstance* jmp = portal(2);
    JumpSetCFrames(jmp, m, n);
    JumpConnectVerses(jmp, self, self);
    nwp = cube(4); {
        PartSetParent(nwp, &jmp->Root);
    }
    
    nwp = cube(2); {
        mat4x4 x;
        mat4x4_mul(x, m, jmp->Root.BoundingBoxCFrame);
        PartSetCFrame(nwp, x);
        PartSetParent(nwp, root);
        
        nwp->DrawMode = GL_LINE_STRIP;
        PartSetSize(nwp, jmp->Root.BoundingBoxSize); 
        PartSetColor(nwp, (color4){0xff, 0, 0, 1});
    }
    
    SignalConnect(&PreRender, updateLight);
}

static void cleanVerse(GLFWwindow* window) {
    DestroyOriginInstance(root);
}