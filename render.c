// Multiverse Delivery_Service Game
// Developed by Gavin Zimmerman

// Render - World Controller


// DEPENDENCIES
#include "linmath.h"
#include "render.h"


// STATE STUFF
VerseInstance Verses[10];

// SOURCE
void renderInit(GLFWwindow* window) {
    // Initialize Verses
}

void renderStep(GLFWwindow* window, double t, double step) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // INPUT
        // Camera Home Verse
        // Camera Home CFrame
    
    // Zoom out from home
    // For each translation Pop onto front of stack
        // TODO Establish method of creating portal geometry (and append to temporary portal list or add to mask)
    
    // From View Verse, search for all surrounding verses/ If in view then add to queue
    // Go until no verses visible 

    // Go through verses backwards and render from translation made
    // 
}

void renderverseex() {
    // IN
        // WorldCFrame
        // Verse to Render
        // Camera Properties

}