// Multi_verse DeliveryService
// Gavin Zimmerman

// Verse#3 - Gate Source
#include "gate.h"

// Shaders

// VERSE
static void buildVerse();
static void cleanVerse();

VerseInstance GATE_VERSE = {
    .Name = "Home",

    .Build = buildVerse,
    .Clean = cleanVerse,
    .RenderStepped = NewSignal,

    .Jumps = {NULL},
};

/*
static void animateJumps() {
    double t = glfwGetTime();

    
    JumpInstance* wf = VerseFindFirstJump(&GATE_VERSE, "waterfall");
    if (wf) {
        wf->Radius = 2+ sin(t/1.f)/5;
        mat4x4 dst;
        mat4x4_look_at(dst, (vec3){0,-2,14}, (vec3){cos(t/2.f)/1.5 + 1, -8, sin(t/1.f)}, UNIT3_Y);
        JumpSetCFrames(wf, wf->CFrame0, dst);
    }
}
static void animateFoam() {
    double t = glfwGetTime();

    PartInstance* cwp;  // Current working part

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam0");
    if (cwp) {
        double a = fmod(t, 1.1) / 1.1;
        double s = ((1-a)* 3) + (a* 6); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam1");
    if (cwp) {
        double a = fmod(t, 1.9) / 1.9;
        double s = ((1-a)* 3) + (a* 5); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam2");
    if (cwp) {
        double a = fmod(t, 2.9) / 2.9;
        double s = ((1-a)* 3) + (a* 5); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam3");
    if (cwp) {
        double a = fmod(t, 4.7) / 4.7;
        double s = ((1-a)* 3) + (a* 6); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam4");
    if (cwp) {
        double a = fmod(t, 2.3) / 2.3;
        double s = ((1-a)* 3) + (a* 6); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220);  
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam5");
    if (cwp) {
        double a = fmod(t, 1.7) / 1.7;
        double s = ((1-a)* 3) + (a* 6); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 

    cwp = VerseFindFirstPart(&GATE_VERSE, "Foam6");
    if (cwp) {
        double a = fmod(t, 1.3) / 1.3;
        double s = ((1-a)* 3) + (a* 6); 
        int r = ((1-a)* 255) + (a* 160); 
        int g = ((1-a)* 255) + (a* 220); 
        PartSetSize(cwp, (vec3){s,s,s});
        PartSetColor(cwp, (color3){r,g,255});
    } 
}


static void animateLights() {
    double t = glfwGetTime();

    static int init=0;
    static vec3 pos[4];
    char str[10]="Light*";
    if (!init) {
        init=1;
        for (int i=0; i<4; i++) {
            str[5]=i+'0';
            PartInstance* cwp = VerseFindFirstPart(&GATE_VERSE, str);
            if (cwp) vec3_set(pos[i], cwp->Position);
        }
    }

    // Set Offsets
    PartInstance* cwp = VerseFindFirstPart(&GATE_VERSE, "Light0");
    if (cwp) {
        vec3 off;
        vec3_add(off, pos[0], (vec3){sin(t/20)*3, sin(t/7)*3, cos(t/5)*3});
        PartSetPosition(cwp, off);
    }
    cwp = VerseFindFirstPart(&GATE_VERSE, "Light1");
    if (cwp) {
        vec3 off;
        vec3_add(off, pos[1], (vec3){cos(t/10)*3, sin(t/71)*2, cos(t/25)*4});
        PartSetPosition(cwp, off);
    }
    cwp = VerseFindFirstPart(&GATE_VERSE, "Light2");
    if (cwp) {
        vec3 off;
        vec3_add(off, pos[2], (vec3){sin(t/20)*2, cos(t/2)*1, sin(t/5)*1});
        PartSetPosition(cwp, off);
    }
    cwp = VerseFindFirstPart(&GATE_VERSE, "Light3");
    if (cwp) {
        vec3 off;
        vec3_add(off, pos[3], (vec3){sin(t/200)*3, cos(t/7)*3, cos(t/15)*2});
        PartSetPosition(cwp, off);
    }
}

void ProjectMatrix(va_list args) {
    GLFWwindow *window = va_arg(args, GLFWwindow*);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    mat4x4_perspective(GATE_VERSE.ProjectMatrix, 60.0*TO_RAD, (double)width/height, 1/10.0, 2000);
}



// SOURCE
// buildVerse
    // @brief Builds HOME_VERSE world
static void buildVerse(VerseInstance* self, GLFWwindow* window) {
    SignalConnect(&self->RenderStepped, ProjectMatrix);

    // Material Building
    PartInstance* nwp; // New Working Part
    PartInstance* rock_cube = cube(1), 
                * water_cube = cube(1),
                * foam = uvSphere(INST_XLO_SEGMTS, INST_XLO_RINGS, 1),
                * sky = plane(2000),
                * rock_pent = cone(5, 4, 2, 1),
                * wood_cube = cube(1),
                * light_cube = cube(.8);
    PartSetShader(rock_cube, EMISSION_SHDR);
    PartSetShader(rock_pent, EMISSION_SHDR);
    PartSetShader(foam, EMISSION_SHDR);
    PartSetShader(water_cube, EMISSION_SHDR);
    PartSetShader(wood_cube, EMISSION_SHDR);
    PartSetShader(light_cube, EMISSION_SHDR);

    // Scene
    // Skybox
    PartSetColor(sky, (color3){0,45,50});
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


    // Scene
    // Rock
    {
        // Platform
        PartSetPivot(rock_cube, PIVOT_TOP);        
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){98,90,105});
            PartSetSize(nwp, (vec3){12,32,20});
            PartSetPosition(nwp, (vec3){-10,-2,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){95,90,100});
            PartSetSize(nwp, (vec3){15,32,17});
            PartSetPosition(nwp, (vec3){10,-2,-1});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){86,70,90});
            PartSetSize(nwp, (vec3){14,10,8});
            PartSetPosition(nwp, (vec3){8,-10,7});
            PartSetRotation(nwp, (vec3){0,-10,0});
        }
        
        // Backwall
        PartSetPivot(rock_cube, PIVOT_BOTTOM);
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){105,90,105});
            PartSetSize(nwp, (vec3){8,16, 8});
            PartSetPosition(nwp, (vec3){-8,-2,-9});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){91,80,100});
            PartSetSize(nwp, (vec3){11,14, 7.5});
            PartSetPosition(nwp, (vec3){8,-2,-9.5});
        }
    }
    {
        // Rock Detail
        PartSetPivot(rock_cube, PIVOT_FORWARD);
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){56,40,60});
            PartSetSize(nwp, (vec3){3,1,1});
            PartSetPosition(nwp, (vec3){8,0,-5.6});
            PartSetRotation(nwp, (vec3){0,-1,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){56,40,66});
            PartSetSize(nwp, (vec3){2,1,1});
            PartSetPosition(nwp, (vec3){12,2,-5.6});
            PartSetRotation(nwp, (vec3){0,3,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){58,42,60});
            PartSetSize(nwp, (vec3){2.6,1.2,1});
            PartSetPosition(nwp, (vec3){10,4,-5.6});
            PartSetRotation(nwp, (vec3){0,-4,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){51,45,62});
            PartSetSize(nwp, (vec3){5,2,1});
            PartSetPosition(nwp, (vec3){9,8,-5.6});
            PartSetRotation(nwp, (vec3){0,0,0});
        }

        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){66,48,60});
            PartSetSize(nwp, (vec3){3,1,1});
            PartSetPosition(nwp, (vec3){-8,1,-4.6});
            PartSetRotation(nwp, (vec3){0,3,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){60,50,66});
            PartSetSize(nwp, (vec3){2,1,1});
            PartSetPosition(nwp, (vec3){-10,6,-4.7});
            PartSetRotation(nwp, (vec3){0,-6,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){58,47,60});
            PartSetSize(nwp, (vec3){2.6,1.2,1});
            PartSetPosition(nwp, (vec3){-9,3,-4.7});
            PartSetRotation(nwp, (vec3){0,0,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){57,45,62});
            PartSetSize(nwp, (vec3){3,1.4,1});
            PartSetPosition(nwp, (vec3){-9,9,-4.6});
            PartSetRotation(nwp, (vec3){0,2,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){57,45,62});
            PartSetSize(nwp, (vec3){1.6,2.2,1});
            PartSetPosition(nwp, (vec3){-6,6,-4.5});
            PartSetRotation(nwp, (vec3){0,-4,0});
        }

        PartSetPivot(rock_cube, PIVOT_TOP);
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){137,125,132});
            PartSetSize(nwp, (vec3){6,.4,5});
            PartSetPosition(nwp, (vec3){-8,-1.92,2});
            PartSetRotation(nwp, (vec3){0,-30,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){137,125,132});
            PartSetSize(nwp, (vec3){6,.4,5});
            PartSetPosition(nwp, (vec3){-9,-1.92,3});
            PartSetRotation(nwp, (vec3){0,-10,0});
        }
        
        
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){169,155,168});
            PartSetSize(nwp, (vec3){4,.4,3});
            PartSetPosition(nwp, (vec3){8,-1.8,5});
            PartSetRotation(nwp, (vec3){0,-24,0});
        }
    }
    {
        PartSetPivot(rock_pent, PIVOT_TOP);
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){169,155,168});
            PartSetSize(nwp, (vec3){5,1,3.6});
            PartSetPosition(nwp, (vec3){0,-2,0});
            PartSetRotation(nwp, (vec3){0,-24,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){91,85,98});
            PartSetSize(nwp, (vec3){2,1.5,3});
            PartSetPosition(nwp, (vec3){1.4,-1.7,.8});
            PartSetRotation(nwp, (vec3){0,8,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){101,85,98});
            PartSetSize(nwp, (vec3){2,2.2,5});
            PartSetPosition(nwp, (vec3){-1.4,5,-5});
            PartSetRotation(nwp, (vec3){90,8,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){121,115,128});
            PartSetSize(nwp, (vec3){10,9,10});
            PartSetPosition(nwp, (vec3){-10,-12,9});
            PartSetRotation(nwp, (vec3){0,8,12});
        }

        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){121,115,128});
            PartSetSize(nwp, (vec3){10,9,10});
            PartSetPosition(nwp, (vec3){-10,-12,9});
            PartSetRotation(nwp, (vec3){0,8,12});
        }
    }

    // Surrounding walls
    {
        PartSetPivot(rock_pent, PIVOT_BOTTOM);
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){71,66,80});
            PartSetSize(nwp, (vec3){30,42,80});
            PartSetPosition(nwp, (vec3){-12,-30,-15});
            PartSetRotation(nwp, (vec3){0,-40,12});
        }
        VerseAddPart(self, nwp = clonePart(rock_pent)); {
            PartSetColor(nwp, (color3){81,66,80});
            PartSetSize(nwp, (vec3){30,32,70});
            PartSetPosition(nwp, (vec3){12,-30,-16});
            PartSetRotation(nwp, (vec3){0,70,12});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){131,126,140});
            PartSetSize(nwp, (vec3){5,5,5});
            PartSetPosition(nwp, (vec3){-12,10,-8});
            PartSetRotation(nwp, (vec3){0,10,-49});
        }
        PartSetPivot(rock_cube, PIVOT_BOTTOM);
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){131,126,140});
            PartSetSize(nwp, (vec3){12,22,8});
            PartSetPosition(nwp, (vec3){-18,-20,-2});
            PartSetRotation(nwp, (vec3){0,30,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){121,106,120});
            PartSetSize(nwp, (vec3){12,14,12});
            PartSetPosition(nwp, (vec3){-18,-20,8});
            PartSetRotation(nwp, (vec3){0,-32,0});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){121,106,130});
            PartSetSize(nwp, (vec3){12,14,12});
            PartSetPosition(nwp, (vec3){18,-20,8});
            PartSetRotation(nwp, (vec3){0,12,5});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){161,146,166});
            PartSetSize(nwp, (vec3){16,16,16});
            PartSetPosition(nwp, (vec3){18,-26,2});
            PartSetRotation(nwp, (vec3){0,30,-12});
        }
        VerseAddPart(self, nwp = clonePart(rock_cube)); {
            PartSetColor(nwp, (color3){151,138,156});
            PartSetSize(nwp, (vec3){14,30,20});
            PartSetPosition(nwp, (vec3){6,-14,-20});
            PartSetRotation(nwp, (vec3){0,60,-2});
        }
    }

    // Water
    {
        PartSetPivot(water_cube, PIVOT_TOP);        
        VerseAddPart(self, nwp = clonePart(water_cube)); {
            PartSetColor(nwp, (color3){0x10, 0xa4, 0xc0});
            PartSetSize(nwp, (vec3){2000,0,2000});
            PartSetPosition(nwp, (vec3){-5,-20,-2});
        }
        VerseAddPart(self, nwp = clonePart(water_cube)); {
            PartSetColor(nwp, (color3){0x12, 0xac, 0xcc});
            PartSetSize(nwp, (vec3){8,20,17.5});
            PartSetPosition(nwp, (vec3){0,-2.6,-.5});
        }
        VerseAddPart(self, nwp = clonePart(water_cube)); {
            PartSetColor(nwp, (color3){0x12, 0xbc, 0xdf});
            PartSetSize(nwp, (vec3){5,15,8});
            PartSetPosition(nwp, (vec3){3,-9.8,7});
            PartSetRotation(nwp, (vec3){0,-10,0});
        }
        PartSetPivot(water_cube, PIVOT_BOTTOM);
        VerseAddPart(self, nwp = clonePart(water_cube)); {
            PartSetColor(nwp, (color3){0x12, 0xbc, 0xcc});
            PartSetSize(nwp, (vec3){6.5,13,20});
            PartSetPosition(nwp, (vec3){-.75,-2.6,-15});
        }
        PartSetPivot(water_cube, PIVOT_TOP);
        VerseAddPart(self, nwp = clonePart(water_cube)); {
            PartSetColor(nwp, (color3){0x12, 0xbc, 0xcc});
            PartSetSize(nwp, (vec3){20,1,12});
            PartSetPosition(nwp, (vec3){-.75,10.4,-18});
        }   
    }

    // Bridge
    {
        // Planks
        {
            PartSetColor(wood_cube, (color3) {140,100,85});
            PartSetSize(wood_cube, (vec3){1,.2,4});
        } partArc(self, wood_cube, (vec3){0,-6.6,2}, (vec3){0,0,0}, (vec3){1,1,1}, 50, 4.8, 10);

        // Support
        {
            PartSetColor(wood_cube, (color3) {120,90,70});
            PartSetSize(wood_cube, (vec3){1.2,.4,.4});
        } partArc(self, wood_cube, (vec3){0,-6.6,3.6}, (vec3){0,0,0}, (vec3){1,1,1}, 50, 4.8, 10);
        {
            PartSetColor(wood_cube, (color3) {120,90,70});
            PartSetSize(wood_cube, (vec3){1.2,.4,.4});
        } partArc(self, wood_cube, (vec3){0,-6.6,.4}, (vec3){0,0,0}, (vec3){1,1,1}, 50, 4.8, 10);
        
        // Handles
        {
            PartSetColor(wood_cube, (color3) {120,90,70});
            PartSetSize(wood_cube, (vec3){1.2,.5,.5});
        } partArc(self, wood_cube, (vec3){0,-4.6,3.6}, (vec3){0,0,0}, (vec3){1,1,1}, 48, 4.8, 10);
        {
            PartSetColor(wood_cube, (color3) {120,90,70});
            PartSetSize(wood_cube, (vec3){1.2,.5,.5});
        } partArc(self, wood_cube, (vec3){0,-4.6,.4}, (vec3){0,0,0}, (vec3){1,1,1}, 48, 4.8, 10);

        // Bars
        {
            PartSetColor(wood_cube, (color3) {110,80,60});
            PartSetSize(wood_cube, (vec3){.4,2,.3});
        } partArc(self, wood_cube, (vec3){0,-5,3.6}, (vec3){0,0,0}, (vec3){1,1,1}, 45, 4.5, 5);
        {
            PartSetColor(wood_cube, (color3) {110,80,60});
            PartSetSize(wood_cube, (vec3){.4,2,.3});
        } partArc(self, wood_cube, (vec3){0,-5,.4}, (vec3){0,0,0}, (vec3){1,1,1}, 45, 4.5, 5);
    }


    // Torii Gate
    {
        mat4x4 R;
        mat4x4_identity(R);
        mat4x4_rotate_Y(R, R, -6*TO_RAD);

        {
            PartSetColor(wood_cube, (color3) {200,40,85});
            PartSetSize(wood_cube, (vec3){.8,1,1.8});
        } partArc(self, wood_cube, (vec3){0,14,1.8}, (vec3){0,80,-180*TO_RAD}, (vec3){1,1,1}, 30, 6, 16);
        
        VerseAddPart(self, nwp = clonePart(wood_cube)); {
            PartSetSize(nwp, (vec3){1,.6,11});
            PartSetPosition(nwp, (vec3){0,3.6,1.8});
            mat4x4_mul(nwp->CFrame, nwp->CFrame, R);
        } 
        // Pillars
        PartSetColor(wood_cube, (color3) {180,30,85});
        VerseAddPart(self, nwp = clonePart(wood_cube)); {
            PartSetSize(nwp, (vec3){.8,9,1});
            PartSetPosition(nwp, (vec3){0,3.6,1.8});
            mat4x4_mul(nwp->CFrame, nwp->CFrame, R);
            mat4x4_translate_in_place(nwp->CFrame, 0, -2.5, -3.2);
        } 
        VerseAddPart(self, nwp = clonePart(wood_cube)); {
            PartSetSize(nwp, (vec3){.8,9,1});
            PartSetPosition(nwp, (vec3){0,3.6,1.8});
            mat4x4_mul(nwp->CFrame, nwp->CFrame, R);
            mat4x4_translate_in_place(nwp->CFrame, 0, -2.5, 3.2);
        } 
    }




    // Random Lights
    {
        VerseAddPart(self, nwp = clonePart(light_cube)); {
            PartSetColor(nwp, (color3){255, 240, 150});
            PartSetPosition(nwp, (vec3){-4,6,3});
            PartSetRotation(nwp, (vec3){300,50,100});
            nwp->Name="Light0";
        } 
        VerseAddPart(self, nwp = clonePart(light_cube)); {
            PartSetColor(nwp, (color3){255, 160, 250});
            PartSetPosition(nwp, (vec3){7,2,9});
            PartSetRotation(nwp, (vec3){20,60,100});
            nwp->Name="Light1";
        }
        VerseAddPart(self, nwp = clonePart(light_cube)); {
            PartSetColor(nwp, (color3){100, 255, 150});
            PartSetPosition(nwp, (vec3){-8,3,0});
            PartSetRotation(nwp, (vec3){100,10,40});
            nwp->Name="Light2";
        }
        VerseAddPart(self, nwp = clonePart(light_cube)); {
            PartSetColor(nwp, (color3){60, 240, 250});
            PartSetPosition(nwp, (vec3){7,4,1});
            PartSetRotation(nwp, (vec3){20,20,200});
            nwp->Name="Light3";
        }
        SignalConnect(&self->RenderStepped, animateLights);
    }


    // Water Foam
    {
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){1,-2.4,-5});
            nwp->Name="Foam0";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){2,-2.6,-5});
            nwp->Name="Foam1";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){-2.5,-2.6,-5.3});
            nwp->Name="Foam2";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){-2,-2.4,-5.3});
            nwp->Name="Foam3";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){3,-2.6,-4.6});
            nwp->Name="Foam4";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){-3,-2.7,-5});
            nwp->Name="Foam5";
        } 
        VerseAddPart(self, nwp = clonePart(foam)); {
            PartSetSize(nwp, (vec3){4,4,4});
            PartSetPosition(nwp, (vec3){.1,-2.7,-4.8});
            nwp->Name="Foam6";
        } 
        SignalConnect(&self->RenderStepped, animateFoam);
    }
    


    // Portals
    {
        mat4x4 src, dst;
        mat4x4_translate(src, 5.6, 3, -5.68);
        mat4x4_look_at(dst, (vec3){0,-2,14}, (vec3){0,-8,0}, UNIT3_Y);
        
        // Waterfall Jump
        JumpInstance* jmp = NewJumpInstance();
        jmp->Name="waterfall";
        jmp->Radius=2;
        JumpSetCFrames(jmp, src, dst);
        JumpConnectVerses(jmp, self, self);
        SignalConnect(&self->RenderStepped, animateJumps);


        // Jump to Home Scene
        jmp = NewJumpInstance();
        jmp->Radius=4;
        mat4x4_translate(src, -11, 5, 15);
        mat4x4_rotate_Y(src, src, 140*TO_RAD);
        mat4x4_rotate_Z(src, src, -10*TO_RAD);
        mat4x4_translate(dst, 0, 0, -15);
        JumpSetCFrames(jmp, src, dst);
        JumpConnectVerses(jmp, self, &HOME_VERSE);


        // Jump to City Scene
        jmp = NewJumpInstance();
        jmp->Radius=10;
        mat4x4_translate(src, 15, 5, 25);
        mat4x4_rotate_Y(src, src, 200*TO_RAD);
        mat4x4_rotate_X(src, src, 3*TO_RAD);
        mat4x4_translate(dst, 0, 0, 20);
        mat4x4_rotate_Y(dst, dst, 180*TO_RAD);
        JumpSetCFrames(jmp, src, dst);
        JumpConnectVerses(jmp, self, &LIGHT_PLAY_VERSE);


        jmp = NewJumpInstance();
        jmp->Radius=5;
        mat4x4_translate(src, 15, 4, 25);
        mat4x4_rotate_Y(src, src, 180*TO_RAD);
        //mat4x4_rotate_X(src, src, 3*TO_RAD);
        mat4x4_translate(dst, 0, 4, 0);
        mat4x4_rotate_Y(dst, dst, 180*TO_RAD);
        JumpSetCFrames(jmp, src, dst);
        JumpConnectVerses(jmp, self, self);
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
}*/