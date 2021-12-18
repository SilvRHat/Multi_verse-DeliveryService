// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#3 - Gate Source
#include "lpg.h"

// Shaders

// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance LIGHT_PLAY_VERSE = {
    .Name = "Lighting",

    .Build = buildVerse,
    .Clean = cleanVerse,
    .RenderStepped = NewSignal,

    .Children = {NULL},
    .Jumps = {NULL},
};

static void buildVerse(VerseInstance* self, GLFWwindow* window) {
    mat4x4_perspective(self->ProjectMatrix, 60.0*TO_RAD, 1080.0/720.0, 1/10.0, 2000);

    // EMISSION SHADER
    unsigned int WORKING_SHADER;
    GLuint frag, vert;
    // Build
    vert = BuildShaderFromFile(GL_VERTEX_SHADER, "shdr/lighting.vert"),
    frag = BuildShaderFromFile(GL_FRAGMENT_SHADER, "shdr/lighting.frag");
    WORKING_SHADER = glCreateProgram();
    glAttachShader(WORKING_SHADER, frag);
    glAttachShader(WORKING_SHADER, vert);
    glLinkProgram(WORKING_SHADER);
    // Cleanup
    glDetachShader(WORKING_SHADER, frag);
    glDetachShader(WORKING_SHADER, vert);
    glDeleteShader(frag);
    glDeleteShader(vert);

    PartInstance* nwp;
    VerseAddPart(self, nwp = cube(1)); {
        PartSetShader(nwp, WORKING_SHADER);
        mat4x4 M;
        mat4x4_identity(M);
        PartSetCFrame(nwp, M);
        nwp->Name="Light test cube";
    }
}


// cleanVerse
    // @brief Cleans up HOME_VERSE
static void cleanVerse(VerseInstance* self, GLFWwindow* window) {
    SignalDestroy(&self->RenderStepped);
    
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        if (self->Children[i]==NULL)
            break;
        self->Children[i] = DestroyPartInstance(self->Children[i]);
    }
}