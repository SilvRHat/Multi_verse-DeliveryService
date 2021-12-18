// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#0 - Home Source
#include "home.h"
#include "camera.h"

// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance HOME_VERSE = {
    .Name = "Home",

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
    mat4x4_perspective(self->ProjectMatrix, 60.0*TO_RAD, 1080.0/720.0, 1/10.0, 2000);

    // Material Building
    PartInstance* nwp = NULL; // New Working Part
    PartInstance* sky = plane(2000),
                * floor_cube = cube(1), 
                * wall_cube = cube(1);


    // Scene
    // Skybox
    PartSetColor(sky, (color3){100,214,240});
    PartSetShader(sky, EMISSION_SHDR);
    for (int i=0; i<3; i++) for (int j=-1; j<=1; j+=2) {
        vec3 pos;
        vec3_set(pos, VEC3_ZERO);

        pos[i]=1000*j;
        // Set Rotations to face inwards
        VerseAddPart(self, nwp = clonePart(sky)); {
            PartSetPosition(nwp, pos);
            switch((i+1)*j) {
                case -3: PartSetRotation(nwp, (vec3){0,90,90}); break;
                case -2: PartSetRotation(nwp, (vec3){0,0,0}); break;
                case -1: PartSetRotation(nwp, (vec3){90,90,0}); break;
                case 1: PartSetRotation(nwp, (vec3){90,-90,0}); break;
                case 2: PartSetRotation(nwp, (vec3){-180,0,0}); break;
                case 3: PartSetRotation(nwp, (vec3){0,-90,90}); break;
            }
        }
    }


    // Floor
    {
        PartSetColor(floor_cube, (color3){227,166,100});
        PartSetShader(floor_cube, EMISSION_SHDR);
        floor_cube->Name="Floor";
        double sx = 1.6, sy = .6;    // Board width, depth
        double py = -3.6;   // Board Y Position

        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 26.2});
            PartSetPosition(nwp, (vec3){sx*12, py, 2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 25.2});
            PartSetPosition(nwp, (vec3){sx*11, py, 3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 26});
            PartSetPosition(nwp, (vec3){sx*10, py, 2.6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 27.8});
            PartSetPosition(nwp, (vec3){sx*9, py, 1.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 2.8});
            PartSetPosition(nwp, (vec3){sx*9, py, -15});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 24.6});
            PartSetPosition(nwp, (vec3){sx*8, py, 3.3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 25.6});
            PartSetPosition(nwp, (vec3){sx*7, py, 2.8});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 26.2});
            PartSetPosition(nwp, (vec3){sx*6, py, 2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 23.8});
            PartSetPosition(nwp, (vec3){sx*5, py, 3.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 25.6});
            PartSetPosition(nwp, (vec3){sx*4, py, 2.8});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 26.2});
            PartSetPosition(nwp, (vec3){sx*3, py, 2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 30});
            PartSetPosition(nwp, (vec3){sx*2, py, .6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 23.2});
            PartSetPosition(nwp, (vec3){sx*1, py, 4});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 27});
            PartSetPosition(nwp, (vec3){sx*0, py, 2.1});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 23.8});
            PartSetPosition(nwp, (vec3){sx*-1, py, 3.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 24.4});
            PartSetPosition(nwp, (vec3){sx*-2, py, 3.4});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 22});
            PartSetPosition(nwp, (vec3){sx*-3, py, 4.6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 20.8});
            PartSetPosition(nwp, (vec3){sx*-4, py, 5.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 1.6});
            PartSetPosition(nwp, (vec3){sx*-4, py, -8.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 17.4});
            PartSetPosition(nwp, (vec3){sx*-5, py, 6.9});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 18.8});
            PartSetPosition(nwp, (vec3){sx*-6, py, 6.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 14.8});
            PartSetPosition(nwp, (vec3){sx*-7, py, 8.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 12.8});
            PartSetPosition(nwp, (vec3){sx*-8, py, 9.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 16.2});
            PartSetPosition(nwp, (vec3){sx*-9, py, 7.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 5.8});
            PartSetPosition(nwp, (vec3){sx*-9, py, -7.3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){sx, sy, 10.4});
            PartSetPosition(nwp, (vec3){sx*-10, py, 10.4});
        }
    }
    
    // Wall
    {
        PartSetColor(wall_cube, (color3){250,186,110});
        PartSetShader(wall_cube, EMISSION_SHDR);
        
        wall_cube->Name="Wall";
        double sx = 1.6, sy = .6;    // Board width, depth
        // Side 1
        {   
            PartSetRotation(wall_cube, (vec3){0,0,90});
            
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 24.6});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*0)+.1, 3.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 27.8});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*1)+.1, 1.7});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 6.8});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*2)+.1, -5});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 10.4});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*2)+.1, 10.4});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 11.8});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*3)+.1, -3.7});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 7});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*3)+.1, 12.1});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 16});
                PartSetPosition(nwp, (vec3){sx*12+ .5, -2.6+(sx*4)+.1, 7.6});
            }
        }
        // Side 2
        {   
            PartSetColor(wall_cube, (color3){240,186,120});
            PartSetRotation(wall_cube, (vec3){0,90,90});
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 24.6});
                PartSetPosition(nwp, (vec3){7.1, -2.5+(sx*0), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 6.8});
                PartSetPosition(nwp, (vec3){-12.6, -2.5+(sx*0), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 15.2});
                PartSetPosition(nwp, (vec3){11.8, -2.5+(sx*1), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 9.4});
                PartSetPosition(nwp, (vec3){-3.1, -2.5+(sx*1), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 8});
                PartSetPosition(nwp, (vec3){15.4, -2.5+(sx*2), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 7.2});
                PartSetPosition(nwp, (vec3){1.2, -2.5+(sx*2), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 23.6});
                PartSetPosition(nwp, (vec3){7.6, -2.5+(sx*3), 15.3});
            }
            VerseAddPart(self, nwp = clonePart(wall_cube)); {
                PartSetSize(nwp, (vec3){sx, sy, 1.8});
                PartSetPosition(nwp, (vec3){18.5, -2.5+(sx*4), 15.3});
            }
        }
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