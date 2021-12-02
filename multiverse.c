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
    EMISSION_SHDR = glCreateProgram();
    glAttachShader(EMISSION_SHDR, BuildShaderFromFile(GL_VERTEX_SHADER, "shdr/emission.vert"));
    glAttachShader(EMISSION_SHDR, BuildShaderFromFile(GL_FRAGMENT_SHADER, "shdr/emission.frag"));
    glBindAttribLocation(EMISSION_SHDR, 1, "Vertex");
    glLinkProgram(EMISSION_SHDR);
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