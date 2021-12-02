// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#0 - Home Source
#include "home.h"


// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance HOME_VERSE = {
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
    mat4x4_perspective(self->ProjectMatrix, 60.0*TO_RAD, asp, 1/10.0, 20);

    // Material Building
    PartInstance* nwp = NULL; // New Working Part
    PartInstance* sky = plane(2000),
                * floor_cube = cube(1), 
                * wall_cube = cube(1);


    // Scene

    // Skybox
    SetColor(sky, (color3){100,214,240});
    SetShader(sky, EMISSION_SHDR);
    for (int i=0; i<3; i++) for (int j=-1; j<=1; j+=2) {
        vec3 pos, rot; 
        vec3_set(pos, VEC3_ZERO);

        pos[i]=1000*j;
        VerseAddChild(self, nwp = clonePart(sky)); {
            SetPosition(nwp, pos);
            switch((i+1)*j) {
                case -3: SetRotation(nwp, (vec3){0,90,90}); break;
                case -2: SetRotation(nwp, (vec3){0,0,0}); break;
                case -1: SetRotation(nwp, (vec3){90,90,0}); break;
                case 1: SetRotation(nwp, (vec3){90,-90,0}); break;
                case 2: SetRotation(nwp, (vec3){-180,0,0}); break;
                case 3: SetRotation(nwp, (vec3){0,-90,90}); break;
            }
        }
    }

    // Floor
    {
        SetColor(floor_cube, (color3){227,166,100});
        SetShader(floor_cube, EMISSION_SHDR);
        
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 26.2});
            SetPosition(nwp, (vec3){21.2,-3.7,2.5});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 25.2});
            SetPosition(nwp, (vec3){19.6,-3.7,3});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 26});
            SetPosition(nwp, (vec3){18,-3.7,2.6});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 27.8});
            SetPosition(nwp, (vec3){16.4,-3.7,1.7});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 24.6});
            SetPosition(nwp, (vec3){14.8,-3.7,3.3});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 25.2});
            SetPosition(nwp, (vec3){13.2,-3.7,2.8});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 26.2});
            SetPosition(nwp, (vec3){11.6,-3.7,2.5});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 23.8});
            SetPosition(nwp, (vec3){10,-3.7,3.7});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 25.6});
            SetPosition(nwp, (vec3){8.4,-3.7,2.8});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 26.2});
            SetPosition(nwp, (vec3){6.8,-3.7,2.5});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 30});
            SetPosition(nwp, (vec3){5.2,-3.7,.6});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 23.2});
            SetPosition(nwp, (vec3){3.6,-3.7,4});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 27});
            SetPosition(nwp, (vec3){2,-3.7,2.1});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 23.8});
            SetPosition(nwp, (vec3){.4,-3.7,3.7});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 24.4});
            SetPosition(nwp, (vec3){-1.2,-3.7,3.4});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 22});
            SetPosition(nwp, (vec3){-2.8,-3.7,4.6});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 1.6});
            SetPosition(nwp, (vec3){-4.4,-3.7,-8.2});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 20.8});
            SetPosition(nwp, (vec3){-4.4,-3.7,5.2});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 17.4});
            SetPosition(nwp, (vec3){-6,-3.7,6.9});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 18.8});
            SetPosition(nwp, (vec3){-7.6,-3.7,6.2});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 14.8});
            SetPosition(nwp, (vec3){-9.2,-3.7,8.2});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 12.8});
            SetPosition(nwp, (vec3){-10.8,-3.7,9.2});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 16.2});
            SetPosition(nwp, (vec3){-12.4,-3.7,7.5});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 5.8});
            SetPosition(nwp, (vec3){-12.4,-3.7,-7.3});
        }
        VerseAddChild(self, nwp = clonePart(floor_cube)); {
            SetSize(nwp, (vec3){1.6,.6, 10.4});
            SetPosition(nwp, (vec3){-14,-3.7, 10.4});
        }
    }
    

    // Wall
    
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