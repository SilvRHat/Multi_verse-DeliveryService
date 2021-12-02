// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#3 - Gate Source
#include "gate.h"


// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance GATE_VERSE = {
    .VerseID = 1,
    .Name = "Home",
    ._Loaded = 0,

    .Build = buildVerse,
    .Clean = cleanVerse,
    .RenderStepped = NewSignal,

    .Children = {NULL},
    .Jumps = {NULL},
};


// SOURCE
// buildVerse
    // @brief Builds HOME_VERSE world
static void buildVerse(VerseInstance* self, GLFWwindow* window) {
    double asp;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    asp = (height>0)? (double)width/height : 1;
    mat4x4_perspective(GATE_VERSE.ProjectMatrix, 60.0*TO_RAD, asp, 1/10.0, 1000);

    // Material Building
    PartInstance* nwp; // New Working Part
    PartInstance* rock_cube = cube(1), 
                * water_cube = cube(1),
                * foam_cube = cube(1);
    SetShader(rock_cube, EMISSION_SHDR);
    SetShader(water_cube, EMISSION_SHDR);
    SetShader(foam_cube, EMISSION_SHDR);


    // Scene
    // Backwall
    VerseAddChild(self, nwp = clonePart(rock_cube)); {
        SetColor(nwp, (color3){98,90,105});
        SetPosition(nwp, (vec3){-5,9,-2});
        SetSize(nwp, (vec3){6,8,4});
    }
    VerseAddChild(self, nwp = clonePart(rock_cube)); {
        SetColor(nwp, (color3){95,90,100});
        SetPosition(nwp, (vec3){4,8.6,-2});
        SetSize(nwp, (vec3){4,7.2,4});
    }

    // Base
    VerseAddChild(self, nwp = clonePart(rock_cube)); {
        SetColor(nwp, (color3){120,110,120});
        SetPosition(nwp, (vec3){-6,0,2.4});
        SetSize(nwp, (vec3){8,10,12.8});
    }
    VerseAddChild(self, nwp = clonePart(rock_cube)); {
        SetColor(nwp, (color3){105,100,110});
        SetPosition(nwp, (vec3){6,0,2.4});
        SetSize(nwp, (vec3){8,10,12.8});
    }

    // Water
    VerseAddChild(self, nwp = clonePart(water_cube)); {
        SetColor(nwp, (color3){0x25,0xb9,0xe1});
        SetPosition(nwp, (vec3){0,7,-2});
        SetSize(nwp, (vec3){8, 9.8, 3.8});
    }
    VerseAddChild(self, nwp = clonePart(water_cube)); {
        SetColor(nwp, (color3){0x25,0xb9,0xe1});
        SetPosition(nwp, (vec3){0,0,2.15});
        SetSize(nwp, (vec3){8, 9.8, 12.6});
    }
    VerseAddChild(self, nwp = clonePart(water_cube)); {
        SetColor(nwp, (color3){0x25,0xb9,0xe1});
        SetPosition(nwp, (vec3){0,-5, 5});
        SetSize(nwp, (vec3){40, 2, 20});
    }

    // Water foam
    SetColor(foam_cube, (color3){250, 252, 255});
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){.9,5,-.6});
        SetSize(nwp, (vec3){1.5,1,1.2});
    }
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){-.3,5,-.7});
        SetSize(nwp, (vec3){1.6,.6,1.5});
    }
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){0,5,-.7});
        SetSize(nwp, (vec3){1.8,.4,1.5});
    }
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){1.4,-4,9});
        SetSize(nwp, (vec3){1.5,1,1.2});
    }
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){-.8,-4,9});
        SetSize(nwp, (vec3){1.6,.6,1.5});
    }
    VerseAddChild(self, nwp = clonePart(foam_cube)); {
        SetPosition(nwp, (vec3){0,-4,9});
        SetSize(nwp, (vec3){1.8,.4,1.5});
    }
}


// cleanVerse
    // @brief Cleans up HOME_VERSE
static void cleanVerse(VerseInstance* self, GLFWwindow* window) {
    SignalDestroy(&self->RenderStepped);
    
    for (int i=0; i<MAX_INSTANCES; i++) {
        if (self->Children[i]==NULL)
            break;
        self->Children[i] = DestroyPartInstance(self->Children[i]);
    }
}