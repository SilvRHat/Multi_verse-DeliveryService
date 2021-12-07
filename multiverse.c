// Multi_verse DeliveryService
// Gavin Zimmerman

// Multiverse - Global Build
#include "multiverse.h"

// SHADERS
unsigned int EMISSION_SHDR = 0;

// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance MULTI_VERSE = {
    .Name = "Multi_verse",

    .Build = buildVerse,
    .Clean = cleanVerse,
    .RenderStepped = NewSignal,

    .Children = {NULL},
    .Jumps = {NULL}
};


// SOURCE
// buildVerse
    // @brief Builds HOME_VERSE world
static void buildVerse(VerseInstance* self, GLFWwindow* window) {
    // EMISSION SHADER
    GLuint emission_frag, emission_vert;
    // Build
    emission_vert = BuildShaderFromFile(GL_VERTEX_SHADER, "shdr/emission.vert"),
    emission_frag = BuildShaderFromFile(GL_FRAGMENT_SHADER, "shdr/emission.frag");
    EMISSION_SHDR = glCreateProgram();
    glAttachShader(EMISSION_SHDR, emission_frag);
    glAttachShader(EMISSION_SHDR, emission_vert);
    glLinkProgram(EMISSION_SHDR);
    // Cleanup
    glDetachShader(EMISSION_SHDR, emission_frag);
    glDetachShader(EMISSION_SHDR, emission_vert);
    glDeleteShader(emission_frag);
    glDeleteShader(emission_vert);
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