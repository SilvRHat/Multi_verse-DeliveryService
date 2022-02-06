// Multi_verse DeliveryService
// Gavin Zimmerman

// Multiverse - Global Build
#include "multiverse.h"

// SHADERS
unsigned int EMISSION_SHDR = 0;

// VERSE
static void buildVerse(GLFWwindow* window);
static void cleanVerse(GLFWwindow* window);

VerseInstance MULTI_VERSE = {
    .Name = "Multi_verse",

    .Build = buildVerse,
    .Clean = cleanVerse,

    .Jumps = {NULL}
};


// SOURCE
// buildVerse
    // @brief Builds HOME_VERSE world
static void buildVerse(GLFWwindow* window) {
    SIMPLE_VERSE.Build(window);
    NOIR_VERSE.Build(window);
}


// cleanVerse
    // @brief Cleans up HOME_VERSE
static void cleanVerse(GLFWwindow* window) {
    SIMPLE_VERSE.Clean(window);
    NOIR_VERSE.Clean(window);
}