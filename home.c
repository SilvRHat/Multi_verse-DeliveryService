// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#0 - Home Source
#include "home.h"


// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance HOME_VERSE = {
    .VerseID = 0,
    .Name = "Home",
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
    double asp;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    asp = (height>0)? (double)width/height : 1;
    mat4x4_perspective(HOME_VERSE.ProjectMatrix, 60.0*TO_RAD, asp, 1/10.0, 20);

    PartInstance* inst = NULL;
    for (int i=0; i<3; i++) for (int j=-1; j<=1; j+=2) {
        vec3 v = {0,0,0};
        v[i]=j*2;
        
        inst = cube(1);
        SetPosition(inst, v);
        SetColor(inst, (color3) {255,200,255});
        VerseAddChild(&HOME_VERSE, inst);
    }
}


// cleanVerse
    // @brief Cleans up HOME_VERSE
static void cleanVerse(VerseInstance self, GLFWwindow* window) {
    SignalDestroy(&self.RenderStepped);
    
    for (int i=0; i<MAX_VERSE_INSTANCES; i++) {
        if (HOME_VERSE.Children[i]==NULL)
            break;
        free(HOME_VERSE.Children[i]);
        HOME_VERSE.Children[i] = NULL;
    }
}