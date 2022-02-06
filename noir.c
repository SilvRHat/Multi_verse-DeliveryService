// Multi_verse DeliveryService
// Gavin Zimmerman
// noirv~

// Noir's Village [City of Sight] - Verse#1
#include "noir.h"


// ALIASES
#define X 0
#define Y 1
#define Z 2


// VERSE
static void buildVerse(GLFWwindow* window);
static void cleanVerse(GLFWwindow* window);
VerseInstance NOIR_VERSE = {
    .Name = "Noir's Village",
    .ClassName = "Verse//1",

    .Build = buildVerse,
    .Clean = cleanVerse,

    .Jumps = {NULL},
    .Root = {
        .Name = "Root",
        .ClassName = "RootOrigin",
        .Parent = NULL,
        .CanRender = GL_TRUE,
        ._Malloced = GL_FALSE,

        .Parts = {NULL},
        .Lights = {NULL},
        .Origins = {NULL}
    }
};

static VerseInstance* self;
static OriginInstance* root;








// SOURCE
// Functions
static void reloadShaders() {
    UpdateShaderFromFile(simpleFragNode(), GL_FRAGMENT_SHADER, "shdr/simple.frag");
}


static void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ((key==GLFW_KEY_R) && (action==GLFW_PRESS))
        reloadShaders();
}


static void onResize(GLFWwindow* window, int width, int height) {
    float asp = (float)width/(float)height;
    mat4x4_perspective(
        self->ProjectionMatrix, 60*TO_RAD, asp, NEAR_PROJECTION, FAR_PROJECTION
    );
}




// Handlers
static void _KeyInputHandler(va_list args) {
    GLFWwindow* window;
    int key, scancode, action, mods;
    {
        window = va_arg(args, GLFWwindow*);
        key = va_arg(args, int);
        scancode = va_arg(args, int);
        action = va_arg(args, int);
        mods = va_arg(args, int);
    }
    onKeyInput(window, key, scancode, action, mods);
}


static void _ResizeHandler(va_list args) {
    GLFWwindow* window;
    int width, height;
    {
        window = va_arg(args, GLFWwindow*);
        width = va_arg(args, int);
        height = va_arg(args, int);
    }
    onResize(window, width, height);
}








// NOIR VERSE
OriginInstance* repeatPart(PartInstance* ref, int num, vec3 range) {
    OriginInstance* parent = NewOriginInstance();

    for (int i=0; i<num; i++) {
        PartInstance* ch = clonePart(ref); {
            vec3 pos;
            vec3_scale(pos, range, (float)i/(float)(num-1));
            PartSetPosition(ch, pos);
            PartSetParent(ch, parent);
        }
    }

    return parent;
}


OriginInstance* roof(PartInstance* ref, float width0, float width1, float depth, float height, float in0, float in1) {
    OriginInstance* parent = NewOriginInstance();

    PartInstance* nwp;

    // Middle
    nwp = clonePart(ref); {
        PartSetSize(nwp, (vec3){depth, in0, width0});
        PartSetPivot(nwp, PIV_TOP);
        PartSetPosition(nwp, (vec3){0.,height,0.});
        PartSetParent(nwp, parent);
    }

    float w2 = (width1-width0)/2;
    float a = atan(w2/height);
    float h = sqrt((w2*w2) + (height*height));

    nwp = clonePart(ref); {
        PartSetSize(nwp, (vec3){depth, h, in1});
        PartSetPivot(nwp, (vec3){0, PIV_TOP[1], PIV_BCK[2]});
        PartSetRotation(nwp, (vec3){a, 0., 0.}, EULER_XYZ);
        PartSetPosition(nwp, (vec3){0., height, -width0/2.});
        PartSetParent(nwp, parent);
    }
    nwp = clonePart(ref); {
        PartSetSize(nwp, (vec3){depth, h, in1});
        PartSetPivot(nwp, (vec3){0, PIV_TOP[1], PIV_FWD[2]});
        PartSetRotation(nwp, (vec3){-a, 0., 0.}, EULER_XYZ);
        PartSetPosition(nwp, (vec3){0., height, width0/2.});
        PartSetParent(nwp, parent);
    }

    return parent;
}



static void lightStepVerse() {

}

static void stepVerse() {
    
}


static void buildVerse(GLFWwindow* window) {
    // INIT
    self = &(NOIR_VERSE);
    root = &(self->Root);
    mat4x4_identity(self->Root.CFrame);
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
         onResize(window, width, height);
    }


    // PATHS / COMMUNICATION
    SignalConnect(&PreRender, stepVerse);               // Pre-Rendering
    SignalConnect(&KeyInput, _KeyInputHandler);          // Key Input
    SignalConnect(&FrameBufferSize, _ResizeHandler);     // Resize



    // SCENE
    PartInstance* nwp = NULL;       // New Working Part
    LightInstance* nwl = NULL;      // New Working Light
    OriginInstance* nwo = NULL;     // New Working Origin
    OriginInstance* form = root;    // Parent


    // COLOR
    OriginInstance* sun = NewOriginInstance(); {
        sun->Name = "Sun";
        OriginSetParent(sun, root);
        {
            nwl = pointLight(0); {
                SignalConnect(&nwl->PrePass, lightStepVerse);
                LightSetParent(nwl, sun);
                nwl->BlendDst = GL_ZERO;
                nwl->BlendSrc = GL_ONE;
            }
        }
    }




    // FLOOR
    nwo = NewOriginInstance(); {
        nwo->Name = "floor";
        OriginSetPosition(nwo, (vec3) {0., -2., 0.} );
        OriginSetRotation(nwo, (vec3) {2.*TO_RAD, 0., 0.}, EULER_XYZ);
        OriginSetParent(nwo, form);
        
        // Parts
        PartInstance* floor_ref = nwp = canvas(0); {
            PartSetRotation(nwp, (vec3) {-90.*TO_RAD, 0., 0.}, EULER_XYZ);
            PartSetColor(nwp, (color4){224, 200, 180, 255});
        }
        PartInstance* gear_ref = nwp = cube(0); {
            PartSetPivot(nwp, (vec3){0., PIV_BOT[1], PIV_BCK[2]});
        }

        // Panel
        vec3 GEAR_RIM_DIM = {0., 1., .75};
        vec3 GEAR_BASE_DIM = {0., .75, 3.4};
        vec3 GEAR_RIDGE_DIM = {3.1, .5, 2.6};
        vec3 GEAR_FLOOR_DIM = {0., .1, 10.};
        
        int PARTS = 16.;
        float GEAR_R = 8.;
        float FLOOR1_W = 100.;
        float FLOOR1_D = 100.;

        // Children
        OriginInstance* form = nwo; {
            // Center Market Floor
            // Long Pieces
            PartSetSize(floor_ref, (vec3) {GEAR_R*2., FLOOR1_D/2. - GEAR_R, 0.});
            nwp = clonePart(floor_ref); {
                PartSetPosition(nwp, (vec3) {0., 0., -GEAR_R});
                PartSetPivot(nwp, PIV_BOT);
                PartSetParent(nwp, form);
            }
            nwp = clonePart(floor_ref); {
                PartSetPosition(nwp, (vec3) {0., 0., GEAR_R});
                PartSetPivot(nwp, PIV_TOP);
                PartSetParent(nwp, form);
            }
            // Outer Tiling
            PartSetSize(floor_ref, (vec3) {FLOOR1_W/2. - GEAR_R, FLOOR1_D, 0.});
            nwp = clonePart(floor_ref); {
                PartSetPosition(nwp, (vec3) {-GEAR_R, 0., 0.});
                PartSetPivot(nwp, PIV_RGT);
                PartSetParent(nwp, form);
            }
            nwp = clonePart(floor_ref); {
                PartSetPosition(nwp, (vec3) { GEAR_R, 0., 0.});
                PartSetPivot(nwp, PIV_LFT);
                PartSetParent(nwp, form);
            }
            // Corners
            float two_sq = 1.4142;
            float r = (two_sq*GEAR_R - GEAR_R) / two_sq;
            PartSetSize(floor_ref, (vec3) {r, r, 0.});
            for (float x=-.5; x<1; x+=1.) for (float y=-.5; y<1; y+=1.) {
                nwp = clonePart(floor_ref); {
                    PartSetPivot(nwp, (vec3){x,y,0.});
                    PartSetPosition(nwp, (vec3){GEAR_R*(x<0?-1:1), 0., GEAR_R*(y<0?1:-1)});
                    PartSetParent(nwp, form);
                }
            }

            // Floor 2
            // North (Royal)
            nwp = clonePart(floor_ref); {
                PartSetSize(nwp, (vec3) {160., 160., 0.});
                PartSetPivot(nwp, (vec3) {PIV_RGT[0]*.5, PIV_BOT[1], 0.});
                PartSetRotation(nwp, (vec3){(-90 +10) * TO_RAD, 0., 20. * TO_RAD}, EULER_XYZ);
                PartSetPosition(nwp, (vec3){FLOOR1_W/2., 0., -FLOOR1_D/2.});
                PartSetParent(nwp, form);
            }
            // South (Sea)
            nwp = clonePart(floor_ref); {
                PartSetSize(nwp, (vec3) {120., 80., 0.});
                PartSetPivot(nwp, (vec3) {PIV_RGT[0], PIV_TOP[1], 0.});
                PartSetRotation(nwp, (vec3){(-90 +5) * TO_RAD, 0., }, EULER_XYZ);
                PartSetPosition(nwp, (vec3){FLOOR1_W/2., 0., FLOOR1_D/2.});
                PartSetParent(nwp, form);
            }
            // East (Waywards)
            nwp = clonePart(floor_ref); {
                PartSetSize(nwp, (vec3) {100., 60., 0.});
                PartSetPivot(nwp, (vec3) {PIV_LFT[0], 0., 0.});
                PartSetPosition(nwp, (vec3){FLOOR1_W/2., 0., 20.});
                PartSetParent(nwp, form);
            }


            // Gear Market Gear
            // Rim
            PartSetSize(gear_ref, (vec3) {(GEAR_R+GEAR_RIM_DIM[2])*2.*M_PI/(float)PARTS, GEAR_RIM_DIM[1], GEAR_RIM_DIM[2]});
            PartSetColor(gear_ref, (color4){255, 150, 140, 255});
            nwo = circlePart(gear_ref, PARTS, 360., GEAR_R, 0.); {
                OriginSetParent(nwo, form);
            }
            // Base
            PartSetSize(gear_ref, (vec3) {(GEAR_R+GEAR_RIM_DIM[2]+GEAR_BASE_DIM[2])*2.*M_PI/(float)PARTS, GEAR_BASE_DIM[1], GEAR_BASE_DIM[2]});
            PartSetColor(gear_ref, (color4){200, 169, 140, 255});
            nwo = circlePart(gear_ref, PARTS, 360., GEAR_R+GEAR_RIM_DIM[2], 0.); {
                OriginSetParent(nwo, form);
            }
            // Ridges
            PartSetSize(gear_ref, GEAR_RIDGE_DIM);
            PartSetColor(gear_ref, (color4){200, 169, 140, 255});
            nwo = circlePart(gear_ref, 16, 620., GEAR_R+GEAR_RIM_DIM[2]+GEAR_BASE_DIM[2]-.2, 0.); {
                OriginSetParent(nwo, form);
            }
            // Circular Floor
            PartSetSize(gear_ref, (vec3) {(GEAR_R+GEAR_RIM_DIM[2]+GEAR_FLOOR_DIM[2])*2.*M_PI/(float)PARTS, GEAR_FLOOR_DIM[1], GEAR_FLOOR_DIM[2]});
            PartSetColor(gear_ref, (color4){200, 169, 140, 255});
            nwo = circlePart(gear_ref, PARTS, 360., GEAR_R+GEAR_RIM_DIM[2], 0.); {
                OriginSetParent(nwo, form);
            }
        }

        DestroyPartInstance(floor_ref);
        DestroyPartInstance(gear_ref);
    }


    // MARKET BUILDINGS
    nwo = NewOriginInstance(); {
        nwo->Name = "market";
        OriginSetParent(nwo, form);

    
        // Children
        OriginInstance* form = nwo; {
            
            // Cafe
            nwo = NewOriginInstance(); {
                OriginSetParent(nwo, form); 
                OriginSetPosition(nwo, (vec3){-30., -3., 22.});
                
                // Parts
                PartInstance* ref = nwp = cube(0.); {
                    PartSetColor(nwp, (color4){239, 140, 98});
                    PartSetPivot(nwp, (vec3){PIV_RGT[0], PIV_BOT[1], 0});
                }

                // Conf
                float WIDTH = 20.;
                float DEPTH = 18.;
                float MID_HEIGHT = 12.;
                float TOT_HEIGHT = 26.;
                float DOOR = -6.5;
                float DOOR_W = 4.5;
                float DOOR_H = 6.;
                float WIN_DEP = .1;
                float ROOF_H = 4.;
                float W = .7;
                float H = 2.;

                // Children
                OriginInstance* form = nwo; {
                    // Base
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){DEPTH,MID_HEIGHT,WIDTH});
                        PartSetPosition(nwp, (vec3){0,0,0});
                        PartSetParent(nwp, form); 
                    }
                    // Upper half
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){DEPTH-3.,TOT_HEIGHT-MID_HEIGHT,WIDTH});
                        PartSetPosition(nwp, (vec3){-3.,MID_HEIGHT,0});
                        PartSetParent(nwp, form); 
                    }
                    // Slant
                    nwp = clonePart(ref); {
                        PartSetPosition(nwp, (vec3){1.,MID_HEIGHT,0.});
                        PartSetSize(nwp, (vec3){4., 8., WIDTH});
                        PartSetRotation(nwp, (vec3){0.,0.,40.*TO_RAD}, EULER_XYZ);
                        PartSetParent(nwp, form);
                    }
                    
                    // Details 
                    PartSetSize(ref, (vec3){W, H, W});
                    nwo = repeatPart(ref, 10, (vec3){0., 0., WIDTH-2.}); {
                        OriginSetPosition(nwo, (vec3){W/2., MID_HEIGHT-3., -(WIDTH-2.)/2.});
                        OriginSetParent(nwo, form);
                    }
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){H/3., H/3., WIDTH+W});
                        PartSetPosition(nwp, (vec3){W/3., MID_HEIGHT-3.+.75, 0});
                        PartSetParent(nwp, form);
                    }

                    // Step
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){2., 1., DOOR_W + 1.});
                        PartSetPosition(nwp, (vec3){2., 0., DOOR});
                        PartSetParent(nwp, form);
                    }

                    // Windows
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){1., DOOR_H, DOOR_W});
                        PartSetPosition(nwp, (vec3){WIN_DEP, 1., DOOR});
                        PartSetParent(nwp, form);
                    }
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){1., 1., DOOR_W});
                        PartSetPosition(nwp, (vec3){WIN_DEP, 1.5+DOOR_H, DOOR});
                        PartSetParent(nwp, form);
                    }

                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){1., 5., WIDTH/2.});
                        PartSetPosition(nwp, (vec3){WIN_DEP, 2., 3.});
                        PartSetParent(nwp, form);
                    }

                    
                    PartSetSize(ref, (vec3){1., (TOT_HEIGHT-MID_HEIGHT)/2., 2.});
                    nwo = repeatPart(ref, 4, (vec3){0.,0.,WIDTH*.75}); {
                        OriginSetPosition(nwo, (vec3){-3.+WIN_DEP, MID_HEIGHT+5.5, -(WIDTH*.75)/2.});
                        OriginSetParent(nwo, form);
                    }

                    // Roof
                    nwo = roof(ref, WIDTH-8., DEPTH-2.5, WIDTH+1., ROOF_H, ROOF_H, 3.5); {
                        OriginSetRotation(nwo, (vec3) {0, 90.*TO_RAD, 0.}, EULER_XYZ);
                        OriginSetPosition(nwo, (vec3){-(DEPTH)/2. -1.5, TOT_HEIGHT, 0.});
                        OriginSetParent(nwo, form);
                    }

                    // Cover
                    nwp = clonePart(ref); {
                        PartSetPivot(nwp, (vec3){PIV_LFT[0], PIV_TOP[1], 0.});
                        PartSetPosition(nwp, (vec3){0., 7.8, 3.});
                        PartSetRotation(nwp, (vec3){0.,0.,-30*TO_RAD}, EULER_XYZ);
                        PartSetSize(nwp, (vec3){4., .2, WIDTH/2.+1.});
                        PartSetParent(nwp, form);
                    }

                    // Tables
                    OriginInstance* tables = NewOriginInstance(); {
                        OriginSetParent(tables, form);
                        OriginSetPosition(tables, (vec3){10., 0., 0.});

                        const int num_tables = 6;
                        vec3 positions[num_tables] = {
                            {25, 0, 1},
                            {13, 0, 5},
                            {-4,-.1, 16},
                            {2, 0, -14},
                            { 8, 0, -6},
                            {0,-.1, 4}
                        };
                        float widths[num_tables] = {
                            5,4.6,4.2,5.2,4.,4.8
                        };
                        // Tables
                        for (int i=0; i<num_tables; i++) {
                            nwo = NewOriginInstance(); {
                                OriginSetPosition(nwo, positions[i]);
                                OriginSetParent(nwo, tables);

                                // Parts
                                float TABLE_H = 2.4;
                                nwp = cylinder(MED_DETAIL, widths[i], .2); {
                                    PartSetPivot(nwp, PIV_BOT);
                                    PartSetPosition(nwp, (vec3){0.,TABLE_H,0.});
                                    PartSetParent(nwp, nwo);
                                }
                                nwp = cylinder(LO_DETAIL, .3, TABLE_H); {
                                    PartSetPivot(nwp, PIV_BOT);
                                    PartSetParent(nwp, nwo);
                                }
                            }
                        }
                    }
                }

                DestroyPartInstance(ref);
            }


            // Gear shop
            nwo = NewOriginInstance(); {
                OriginSetParent(nwo, form);
                OriginSetPosition(nwo, (vec3){-31,-3,2});

                // Conf
                float BASE_D = 18.;
                float BASE_W = 18.;
                float MID_H = 12.;
                float TOT_H = 20.;
                float OUT_D = 3.;
                float ANG = 60.;
                // DOOR
                float DOOR_W = 4.;
                float DOOR_H = 7.;
                float DOOR_P = 4.4;
                float WIN_D = .1;
                float WIN0_W = 7.;
                float WIN0_H = 5.;
                // Sign Detail
                float SIGN_WP = 2.;
                float SIGN_HP = 10.;
                float SIGN_D = 6.;
                // Roof Detail
                float ROOFD_W = 9.;
                float ROOFD_D = 4.;
                float ROOFD_H0 = 2.6;
                float ROOFD_H1 = 4.;
                float ROOFD_H2 = .6;


                // Parts
                PartInstance* ref = nwp = cube(0.); {
                    PartSetColor(nwp, (color4){239, 140, 98});
                    PartSetPivot(nwp, (vec3){PIV_RGT[0], PIV_BOT[1], 0});
                }
                PartInstance* ref1 = cylinder(MED_DETAIL, ROOFD_D*2./7., ROOFD_H1); {
                    PartSetPivot(ref1, PIV_BOT);
                }

                // Children
                OriginInstance* form = nwo; {
                    // Base Building
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){BASE_D, MID_H, BASE_W});
                        PartSetPosition(nwp, (vec3){0,0,0});
                        PartSetParent(nwp, form); 
                    }
                    // Top Building
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){BASE_D+OUT_D, TOT_H-MID_H, BASE_W});
                        PartSetPosition(nwp, (vec3){OUT_D, MID_H, 0.});
                        PartSetParent(nwp, form);
                    }
                    // Inlet
                    nwp = clonePart(ref); {
                        float h = fabs(OUT_D/ sin(-ANG*TO_RAD));
                        PartSetPivot(nwp, (vec3){PIV_RGT[0],PIV_TOP[1],0.});
                        PartSetSize(nwp, (vec3){h, h, BASE_W});
                        PartSetPosition(nwp, (vec3){OUT_D, MID_H, 0.});
                        PartSetRotation(nwp, (vec3){0.,0.,-ANG*TO_RAD}, EULER_XYZ);
                        PartSetParent(nwp, form);
                    }
                    nwp = clonePart(ref); {
                        float a=.4;
                        PartSetPosition(nwp, (vec3){a/2. + OUT_D, MID_H, 0.});
                        PartSetSize(nwp, (vec3){BASE_D+OUT_D+a, a*2, BASE_W+a});
                        PartSetParent(nwp, form);
                    }
                    nwp = clonePart(ref); {
                        float a=.4;
                        PartSetPosition(nwp, (vec3){a/2. + OUT_D, TOT_H, 0.});
                        PartSetSize(nwp, (vec3){BASE_D+OUT_D+a, a*2, BASE_W+a});
                        PartSetParent(nwp, form);
                    }

                    // Beams
                    PartSetSize(ref, (vec3){1.,MID_H, 1.});
                    nwo = repeatPart(ref, 2, (vec3){0.,0.,BASE_W-2.}); {
                        OriginSetPosition(nwo, (vec3){1.5,0.,-(BASE_W-2.)/2.});
                        OriginSetParent(nwo, form);
                    }

                    // Door
                    float DOOR_STEP_D = 2.6;
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){DOOR_STEP_D, 1., DOOR_W+1.});
                        PartSetPosition(nwp, (vec3){DOOR_STEP_D,0,DOOR_P});
                        PartSetParent(nwp, form); 
                    }
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){1., DOOR_H, DOOR_W});
                        PartSetPosition(nwp, (vec3){WIN_D, 1., DOOR_P});
                        PartSetParent(nwp, form);
                    }

                    // Windows
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){1., WIN0_H, WIN0_W});
                        PartSetPosition(nwp, (vec3){WIN_D, 3., -3.});
                        PartSetParent(nwp, form);
                    }

                    PartSetSize(ref, (vec3){1., 4.6, 3.});
                    nwo = repeatPart(ref, 3, (vec3){0., 0., BASE_W-6.}); {
                        OriginSetParent(nwo, form);
                        OriginSetPosition(nwo, (vec3){WIN_D+OUT_D, MID_H+2., -(BASE_W-6.)/2});
                    }

                    nwp = cylinder(LO_DETAIL, .2, SIGN_D); {
                        PartSetPivot(nwp, PIV_BOT);
                        PartSetPosition(nwp, (vec3){0., SIGN_HP, SIGN_WP});
                        PartSetRotation(nwp,(vec3){0., 0., -90.*TO_RAD}, EULER_XYZ);
                        PartSetParent(nwp, form);
                    }

                    nwp = clonePart(ref); {
                        PartSetPivot(nwp, (vec3){PIV_LFT[0], PIV_TOP[1], 0.});
                        PartSetSize(nwp, (vec3){4., 1.6, .1});
                        PartSetPosition(nwp, (vec3){2., SIGN_HP-.5, SIGN_WP});
                        PartSetParent(nwp, form);
                        PartSetRotation(nwp, (vec3){3*TO_RAD,0.,0.}, EULER_XYZ);
                    }

                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){ROOFD_D, ROOFD_H0, ROOFD_W});
                        PartSetPosition(nwp, (vec3){1., TOT_H, -3.});
                        PartSetParent(nwp, form);
                    }
        
                    nwo = repeatPart(ref1, 3, (vec3){0.,0.,ROOFD_W/2.}); {
                        OriginSetPosition(nwo, (vec3){1.-(ROOFD_D/2.), TOT_H+ROOFD_H0, -3.-(ROOFD_W/2./2)});
                        OriginSetParent(nwo, form);
                    }

                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){ROOFD_D*.9, ROOFD_H2, ROOFD_W*.9});
                        PartSetPosition(nwp, (vec3){1., TOT_H+ROOFD_H1+ROOFD_H0, -3.});
                        PartSetParent(nwp, form);
                    }
                }

                DestroyPartInstance(ref);
                DestroyPartInstance(ref1);
            }
        

            // Hotel
            nwo = NewOriginInstance(); {
                nwo->Name = "hotel";
                OriginSetParent(nwo, form);
                OriginSetPosition(nwo, (vec3){-31,-3,-21});

                // Conf
                float BASE_W = 26.;
                float BASE_D = 18.;
                float BASE_H = 20.;
                float ROOF_H = 7.;
                float ROOF_I = 10.;
                float ROOF_O = 1.;
                int ROOF_NUM = 6;

                // Parts
                PartInstance* ref = nwp = cube(0.); {
                    PartSetColor(nwp, (color4){239, 140, 98});
                    PartSetPivot(nwp, (vec3){PIV_RGT[0], PIV_BOT[1], 0});
                }

                // Children
                OriginInstance* form = nwo; {
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){BASE_D, BASE_H, BASE_W});
                        PartSetPosition(nwp, (vec3){0,0,0});
                        PartSetParent(nwp, form); 
                    }
                    nwp = clonePart(ref); {
                        float u = .25;
                        float v = u*3.;
                        PartSetSize(nwp, (vec3){BASE_D+u, v, BASE_W+u});
                        PartSetPosition(nwp, (vec3){u/2., 12., 0.});
                        PartSetParent(nwp, form);
                    }
                }

                DestroyPartInstance(ref);
            }


            // Train Station
            OriginInstance* station = nwo = NewOriginInstance(); {
                nwo->Name="station";
                OriginSetPosition(nwo, (vec3){-30,-3,-46});
                OriginSetParent(nwo, form);
                
                // Conf
                 float BASE_Z = 23.;
                float BASE_Y = 30.;
                float BASE_X = 18.;
                float ROOF_H = 15.;

                // Parts
                PartInstance* ref_cube = nwp = cube(0.); {
                    nwp->Pivot[Y] = PIV_BOT[Y];
                    nwp->Pivot[X] = PIV_RGT[X];
                }
                PartInstance* ref_cyl0 = nwp = cylinder(HI_DETAIL, 0., 0.); {
                    nwp->Pivot[X] = PIV_RGT[X];
                }

                // Children
                OriginInstance* form = nwo; {
                    nwp = clonePart(ref_cube); {
                        PartSetSize(nwp, (vec3){BASE_X, BASE_Y, BASE_Z});
                        PartSetParent(nwp, form);
                    }
                    nwp = clonePart(ref_cyl0); {
                        PartSetPosition(nwp, (vec3){0., BASE_Y, 0.});
                        PartSetRotation(nwp, (vec3){90.*TO_RAD, 0., 0.}, EULER_XYZ);
                        PartSetSize(nwp, (vec3){BASE_X, BASE_Z-5e-3, ROOF_H});
                        PartSetParent(nwp, form);
                    }

                    // Clone
                        nwo = cloneOrigin(station); {
                        OriginSetPosition(nwo, (vec3){-BASE_X -1., -0., 0.});
                        OriginSetParent(nwo, form);
                    }
                }

                DestroyPartInstance(ref_cube);
                DestroyPartInstance(ref_cyl0);
            }
            

            // Clock Tower
            nwo = NewOriginInstance(); {
                nwo->Name="clocktower";
                OriginSetPosition(nwo, (vec3){17., -2., -30.});
                OriginSetRotation(nwo, (vec3){0., -20*TO_RAD, 0.}, EULER_XYZ);
                OriginSetParent(nwo, form);
                
                // Conf
                float WIDTH = 10.;
                float HEIGHT = 50.;

                // Children
                OriginInstance* form = nwo; {
                    // Main Tower
                    nwp = cube(0.); {
                        PartSetPivot(nwp, PIV_BOT);
                        PartSetSize(nwp, (vec3){WIDTH, HEIGHT, WIDTH});
                        PartSetParent(nwp, form);
                    }
                }
            }


            // Posuka Building
            nwo = NewOriginInstance(); {
                nwo->Name="bookstore";
                OriginSetParent(nwo, form);
                OriginSetPosition(nwo, (vec3){22,0,-50});

                // Conf
                float WIDTH = 22.;
                float DEPTH = 18.;
                float HEIGHT = 17.;

                // Parts
                PartInstance* ref = nwp = cube(0.); {
                    PartSetColor(nwp, (color4){239, 140, 98});
                    PartSetPivot(nwp, (vec3){PIV_LFT[0], PIV_BOT[1], 0});
                }

                // Children
                OriginInstance* form = nwo; {
                    nwp = clonePart(ref); {
                        PartSetSize(nwp, (vec3){DEPTH,HEIGHT,WIDTH});
                        PartSetPosition(nwp, (vec3){0,0,0});
                        PartSetParent(nwp, form); 
                    }
                }

                DestroyPartInstance(ref);
            }
        }
    }


    // BRIDGE
    nwo = NewOriginInstance(); {
        nwo->Name = "bridge";
        OriginSetPosition(nwo, (vec3){30., -6., 0.});
        OriginSetParent(nwo, form);

        // Conf
        float BRIDGE_H = 26.;
        float BRIDGE_X = 36.;
        float PLATFRM_Y = 2.;

        // Parts
        PartInstance* ref0 = cube(0.); {
            ref0->Pivot[X] = PIV_LFT[X];
            ref0->Pivot[Y] = PIV_BOT[Y];
            ref0->Pivot[Z] = PIV_FWD[Z];
        }

        // Children
        OriginInstance* form = nwo; {
            // South
            nwo = NewOriginInstance(); {
                nwo->Name="south";
                OriginSetPosition(nwo, (vec3){0.,0., 54.});
                OriginSetParent(nwo, form);

                // Conf
                float WALL_Z = 20.;

                // Children
                OriginInstance* form = nwo; {
                    nwp = clonePart(ref0); {
                        PartSetSize(nwp, (vec3){BRIDGE_X, BRIDGE_H-PLATFRM_Y, WALL_Z});
                        PartSetParent(nwp, form);
                    }

                    // Bridge Cafe
                    nwo = NewOriginInstance(); {
                        nwo->Name="cafe";
                        OriginSetPosition(nwo, (vec3){1., 0., -30.});
                        OriginSetParent(nwo, form);

                        // Conf
                        float CAFE_D = 30.;
                        float CAFE_H = 14.;
                        float CAFE_W = 20.;
                        float BEAM_W = 2.;

                        // Children
                        OriginInstance* form = nwo; {
                            ref0->Pivot[Z] = PIV_BCK[Z];
                            // Base
                            nwp = clonePart(ref0); {
                                PartSetSize(nwp, (vec3){CAFE_D, CAFE_H, CAFE_W});
                                PartSetParent(nwp, form);
                            }
                            // Roof
                            nwp = clonePart(ref0); {
                                PartSetPosition(nwp, (vec3){0., CAFE_H, -.5});
                                PartSetSize(nwp, (vec3){CAFE_D, 5., 15.});
                                PartSetRotation(nwp, (vec3){-20*TO_RAD, 0., 0.}, EULER_XYZ);
                                nwp->Pivot[Y] = PIV_TOP[Y];
                                PartSetParent(nwp, form);
                            }
                            // Beams
                            PartSetSize(ref0, (vec3){BEAM_W, BRIDGE_H-PLATFRM_Y-CAFE_H, BEAM_W});
                            nwo = repeatPart(ref0, 3, (vec3){CAFE_D*.8, 0., 0.}); {
                                OriginSetPosition(nwo, (vec3){2., CAFE_H, 4.});
                                OriginSetParent(nwo, form);
                            }
                            // Doors
                            nwp = clonePart(ref0); {

                            }
                        }
                    }
                }
            }

            // North
            nwo = NewOriginInstance(); {
                nwo->Name="north";
                OriginSetPosition(nwo, (vec3){0.,0., -10.});
                OriginSetParent(nwo, form);

                // Conf
                float WALL_Z = 140.;

                // Children
                OriginInstance* form = nwo; {
                    ref0->Pivot[Z] = PIV_FWD[Z];
                    nwp = clonePart(ref0); {
                        PartSetSize(nwp, (vec3){BRIDGE_X, BRIDGE_H-PLATFRM_Y, WALL_Z});
                        PartSetParent(nwp, form);
                    }
                }
            }
        }
    }


    // SCENERY
    nwo = NewOriginInstance(); {
        OriginSetPosition(nwo, (vec3) {0., -30., 0.});
        OriginSetParent(nwo, root);


        // Children
        OriginInstance* form = nwo; {
            // OCEAN
            nwp = circle(HI_DETAIL, 1000); {
                PartSetRotation(nwp, (vec3){-90.*TO_RAD, 0., 0.}, EULER_XYZ);
                PartSetParent(nwp, form);
            }
        }
    }
}

// CLEANUP
static void cleanVerse(GLFWwindow* window) {
    DestroyOriginInstance(root);
}