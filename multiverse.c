// Multi_verse DeliveryService
// Gavin Zimmerman

// Multiverse - Global Build
#include "multiverse.h"

// SHADERS
int EMISSION_SHDR = 0;

// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance MULTIVERSE = {
    .VerseID = 0,
    .Name = "Multi_verse",
    ._Loaded = 0,

    .Build = buildVerse,
    .Clean = cleanVerse,
    .RenderStepped = NewSignal,

    .Children = {NULL},
    .Jumps = {NULL}
};


// SOURCE
// buildVerse
    // @brief Builds HOME_VERSE world
static void buildVerse(VerseInstance self, GLFWwindow* window) {
    // EMISSION SHADER
    int prog = glCreateProgram();
    glAttachShader(prog, BuildShader(GL_VERTEX_SHADER, "shdr/emission.vert"));
    glAttachShader(prog, BuildShader(GL_FRAGMENT_SHADER, "shdr/emission.frag"));
    glLinkProgram(prog);
    EMISSION_SHDR = prog;

    
}


// cleanVerse
    // @brief Cleans up HOME_VERSE
static void cleanVerse(VerseInstance self, GLFWwindow* window) {
    SignalDestroy(&self.RenderStepped);
    
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        if (MULTIVERSE.Children[i]==NULL)
            break;
        free(MULTIVERSE.Children[i]);
        MULTIVERSE.Children[i] = NULL;
    }
}