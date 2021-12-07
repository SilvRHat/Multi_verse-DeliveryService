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

void DrawVerseAt(VerseInstance* self, mat4x4 M, color3 c) {
    PartInstance* nwp = NULL;
    mat4x4 rot;
    vec4 look, pos, poslook, temp;
    mat4x4_from_rot(rot, M);
    mat4x4_col(look, M, 2);
    mat4x4_col(pos, M, 3);

    mat4x4_rotate_X(rot, rot, 90*TO_RAD);
    
    void* lineDrawFunc;
    vec3_scale(temp, look, 2);
    vec3_add(poslook, temp, pos);
    VerseAddPart(self, nwp = line(2, (vec4[]){
            {pos[0],pos[1],pos[2],1}, 
            {poslook[0],poslook[1],poslook[2],1}
        })); {
        PartSetShader(nwp, EMISSION_SHDR);
    }
    lineDrawFunc = nwp->Draw;

    VerseAddPart(self, nwp = circle(INST_MED_RINGS, 5)); {
        PartSetCFrame(nwp, rot);
        PartSetPosition(nwp, pos);
        PartSetShader(nwp, EMISSION_SHDR);
        PartSetColor(nwp, c);
        nwp->Draw = lineDrawFunc;
    }
}


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
    /*for (int i=0; i<3; i++) for (int j=-1; j<=1; j+=2) {
        vec3 pos, rot; 
        vec3_set(pos, VEC3_ZERO);

        pos[i]=1000*j;
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
    }*/

    // Add in Jumps
    mat4x4 CFA, CFB, M;
    mat4x4_translate(CFA, -5,0,0);
    mat4x4_rotate_Y(CFA, CFA, TO_RAD*90);
    mat4x4_identity(CFB);
    mat4x4_rotate_Y(CFB, CFB, 180*TO_RAD);
    mat4x4_translate_in_place(CFB, -30,0,0);
    mat4x4_rotate_Y(CFB, CFB, TO_RAD*80);

    // Make a jump
    JumpInstance *j;
    VerseAddJump(self, j = NewJumpInstance()); {
        JumpSetCFrames(j, CFA, CFB);
        JumpConnectVerses(j, self, self);
        j->Radius=5;
    }
    DrawVerseAt(self, CFA, (color3){255,0,0});
    DrawVerseAt(self, CFB, (color3){0,255,0});

    
    

    // Floor
    {
        PartSetColor(floor_cube, (color3){227,166,100});
        PartSetShader(floor_cube, EMISSION_SHDR);
        
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 26.2});
            PartSetPosition(nwp, (vec3){21.2,-3.7,2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 25.2});
            PartSetPosition(nwp, (vec3){19.6,-3.7,3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 26});
            PartSetPosition(nwp, (vec3){18,-3.7,2.6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 27.8});
            PartSetPosition(nwp, (vec3){16.4,-3.7,1.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 24.6});
            PartSetPosition(nwp, (vec3){14.8,-3.7,3.3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 25.2});
            PartSetPosition(nwp, (vec3){13.2,-3.7,2.8});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 26.2});
            PartSetPosition(nwp, (vec3){11.6,-3.7,2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 23.8});
            PartSetPosition(nwp, (vec3){10,-3.7,3.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 25.6});
            PartSetPosition(nwp, (vec3){8.4,-3.7,2.8});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 26.2});
            PartSetPosition(nwp, (vec3){6.8,-3.7,2.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 30});
            PartSetPosition(nwp, (vec3){5.2,-3.7,.6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 23.2});
            PartSetPosition(nwp, (vec3){3.6,-3.7,4});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 27});
            PartSetPosition(nwp, (vec3){2,-3.7,2.1});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 23.8});
            PartSetPosition(nwp, (vec3){.4,-3.7,3.7});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 24.4});
            PartSetPosition(nwp, (vec3){-1.2,-3.7,3.4});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 22});
            PartSetPosition(nwp, (vec3){-2.8,-3.7,4.6});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 1.6});
            PartSetPosition(nwp, (vec3){-4.4,-3.7,-8.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 20.8});
            PartSetPosition(nwp, (vec3){-4.4,-3.7,5.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 17.4});
            PartSetPosition(nwp, (vec3){-6,-3.7,6.9});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 18.8});
            PartSetPosition(nwp, (vec3){-7.6,-3.7,6.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 14.8});
            PartSetPosition(nwp, (vec3){-9.2,-3.7,8.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 12.8});
            PartSetPosition(nwp, (vec3){-10.8,-3.7,9.2});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 16.2});
            PartSetPosition(nwp, (vec3){-12.4,-3.7,7.5});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 5.8});
            PartSetPosition(nwp, (vec3){-12.4,-3.7,-7.3});
        }
        VerseAddPart(self, nwp = clonePart(floor_cube)); {
            PartSetSize(nwp, (vec3){1.6,.6, 10.4});
            PartSetPosition(nwp, (vec3){-14,-3.7, 10.4});
        }
    }
    

    // Wall
    
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