// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"

// Globals
static vec4 ClipPlane = VEC4_ZERO;

/*
// SOURCE
void renderInstanceGeometry(PartInstance *p, mat4x4 ProjectMat, mat4x4 ViewMat) {
    unsigned int vao, program, loc;
    mat4x4 ModelViewMat, NormalMat4;
    float NormalMat[3][3];

    vao = p->Vao;
    program = p->Shader;
    if (!(vao && program && p->Vbo)) {
        printf("Cannot render geometry %s\n", p->Name);
        return;
    }
    
    
    //  Bind VAO
    glUseProgram(program);
    glBindVertexArray(vao);
   
    //  Set Projection and Model View Matrix
    mat4x4_mul(ModelViewMat, ViewMat, p->CFrame);
    mat4x4_invert(NormalMat4, ModelViewMat);
    mat4x4_transpose(NormalMat4, NormalMat4);
    for (int i=0; i<3; i++) vec3_dup(NormalMat[i], NormalMat4[i]);
    mat4x4_scale_aniso(ModelViewMat, ModelViewMat, (vec3) {p->Size[0], p->Size[1], p->Size[2]});

    

    loc = glGetUniformLocation(program,"ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ProjectMat);
    loc = glGetUniformLocation(program,"ModelViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ModelViewMat);
    loc = glGetUniformLocation(program,"NormalMatrix");
    glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat *)NormalMat);
    loc = glGetUniformLocation(program,"ViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ViewMat);

    //  Set color
    loc = glGetUniformLocation(program, "Color");
    glUniform4fv(loc, 1, (const GLfloat *) p->Color);

    loc = glGetUniformLocation(program, "ClipPlane");
    glUniform4fv(loc, 1, (const GLfloat *) ClipPlane);

    // Call instance drawing operations
    glDrawArrays(p->DrawType, 0, p->Vertices);
}


void renderJumpGeometry(JumpRenderData* rj, int renderAll) {
    // Draw Circle
    for (int i=(renderAll)?0:rj->Depth; i<=rj->Depth; i++) {
        JumpRenderData* wrj = (i<rj->Depth)? rj->Travel[i] : rj;
        PartInstance *portal = circle(INST_HI_SEGMTS, wrj->Radius); {
            mat4x4 CF;
            mat4x4_identity(CF);
            mat4x4_rotate_X(CF, CF, 90*TO_RAD);
            PartSetShader(portal, EMISSION_SHDR);
            PartSetCFrame(portal, CF);
        }
        renderInstanceGeometry(portal, wrj->VerseTo->ProjectMatrix, wrj->OffsetFrom);
        DestroyPartInstance(portal);
    }
}


// Clipping Method Credit: https://prideout.net/clip-planes
void setClipPlaneToJump(JumpRenderData* rj) {
    vec4 look, pos;
    vec3 dist;
    mat4x4 JumpCF;

    // Get Position and Look Vector of Jump location using ModelView Space
    mat4x4_dup(JumpCF, rj->OffsetFrom);
    mat4x4_col(pos, JumpCF, 3);
    mat4x4_col(look, JumpCF, 2);
    
    // Get the distance of the plane from the origin
    line_plane_intersection(dist, VEC4_ZERO, look, pos, look);
    look[3] = vec3_length(dist);

    // Flip Plane Normal and set Clipping Plane
    vec4_scale(ClipPlane, look, -1);
}


void renderVerse(GLFWwindow* window, JumpRenderData* jump, double t, double step) {
    VerseInstance* verse = jump->VerseTo;

    SignalFire(&verse->RenderStepped, window, t, step);
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        PartInstance* p = verse->Root.Parts[i];
        if (p==NULL)
            break;
        // Draw Instance
        renderInstanceGeometry(p, verse->ProjectMatrix, jump->OffsetTo);
    }
}


void renderStep(GLFWwindow* window, double t, double step) {
    JumpRenderData VerseRenderStack[MAX_RENDER_VERSES] = {{
        .Depth = 0, 
        .Travel = {NULL},
        .Radius = 0,

        .VerseTo = NULL,

        .Bounds = {-1, 1, -1, 1},
        .Z_Bound = 0
    }};
    int stackpos=0;
    
    
    // Start render stack from cameraview using camera
    GetCameraView(VerseRenderStack[0].OffsetTo, &(VerseRenderStack[0].VerseTo));
    // Get Camera offset from origin by inverting matrix
    mat4x4_invert(VerseRenderStack[0].OffsetTo, VerseRenderStack[0].OffsetTo);


    // Build order of scenes to render from jumps/ portals  (build render stack)
    for (int i=0; (i<MAX_RENDER_VERSES) && (stackpos+1<MAX_RENDER_VERSES); i++) {
        VerseInstance* VerseToDraw = VerseRenderStack[i].VerseTo;
        if (VerseToDraw==NULL)
            break;

        JumpRenderData* JumpDataA = &(VerseRenderStack[i]);
        JumpRenderData* JumpDataB;


        // Go through all jumps in current VerseToDraw scene and add neighboring verses to stack
        for (int j=0; j<MAX_JUMP_INSTANCES; j++) {
            JumpInstance* jmp = VerseToDraw->Jumps[j];
            if ((jmp==NULL) || (!(stackpos+1<MAX_RENDER_VERSES))) 
                break;
            
            // Try both directions (Verse0->Verse1) (Verse1->Verse0)
            for (int k=0; k<2; k++) {
                // From [Labeled A]; To [Labeled B]
                VerseInstance* vA = (!k)? jmp->Verse0 : jmp->Verse1;
                VerseInstance* vB = (!k)? jmp->Verse1 : jmp->Verse0;

                // Check if jump is valid
                if (vA!=VerseToDraw) continue;  // Direction must start in verse to render
                if (!(JumpDataA->Depth<MAX_RENDER_JUMP_DEPTH)) continue;  // Must not exceed the maximum depth
                if (!(stackpos+1<MAX_RENDER_VERSES)) break; // Cannot exceed total verse render stack

                // Check if visible
                vec4 NewBounds;
                float NewZDepth;
                mat4x4 jmpCF;
                {
                    vec4 pos, look, diff;
                    mat4x4_mul(jmpCF, JumpDataA->OffsetTo, (!k)? jmp->CFrame0 : jmp->CFrame1);
                    
                    // Simple Testing
                    mat4x4_col(pos, jmpCF, 3); 
                    mat4x4_col(look, jmpCF, 2); 
                    vec3_norm(diff, pos); // Get unit vector to object in world-view
                    NewZDepth = pos[2];

                    if (NewZDepth > JumpDataA->Z_Bound) continue;  // Simple bounds check on depth
                    if (vec3_mul_inner(look, diff)>=0) continue;  // 'Face cull' portal


                    // Thorough Testing: Check if visible through all previous portals
                    // Use procedural NDC-space bounds, if portal falls outside bounds ignore it
                    mat4x4 ToClipSpace, BoundBox;
                    mat4x4 bM,tM,lM,rM;
                    double rad = jmp->Radius;
                    mat4x4_identity(BoundBox);
                    mat4x4_mul(ToClipSpace, vA->ProjectMatrix, jmpCF);
                    vec2_dup(BoundBox[3], (vec2){-rad, 0}); mat4x4_mul(lM, ToClipSpace, BoundBox);  // Left
                    vec2_dup(BoundBox[3], (vec2){+rad, 0}); mat4x4_mul(rM, ToClipSpace, BoundBox);  // Right
                    vec2_dup(BoundBox[3], (vec2){0, -rad}); mat4x4_mul(bM, ToClipSpace, BoundBox);  // Bottom
                    vec2_dup(BoundBox[3], (vec2){0, +rad}); mat4x4_mul(tM, ToClipSpace, BoundBox);  // Top
                    
                    vec4 b0, b1; // Previous bounds, Current Bounds
                    vec4_dup(b0, JumpDataA->Bounds);
                    // Set bounds in NDC-space; let each matrix have a different W-value
                    vec4_dup(b1, (vec4){lM[3][0]/lM[3][3], rM[3][0]/rM[3][3], bM[3][1]/bM[3][3], tM[3][1]/tM[3][3]});
                    
                    // Bounds check
                    if (
                        (b0[0]>=b1[1]) || (b0[1]<=b1[0]) ||
                        (b0[2]>=b1[3]) || (b0[3]<=b1[2])
                    ) continue;
                    // Reconstrain bounds
                    vec4_dup(NewBounds, (vec4){max(b0[0],b1[0]), min(b0[1],b1[1]), max(b0[2],b1[2]), min(b0[3],b1[3])});
                }
                
                // Passed Validation
                // Add new verse to rendering stack
                stackpos++;
                JumpDataB = &(VerseRenderStack[stackpos]);
                
                // Book Keeping
                JumpDataB->Depth = JumpDataA->Depth+1;
                for (int s=0; s<JumpDataA->Depth; s++)
                    JumpDataB->Travel[s]=JumpDataA->Travel[s];
                JumpDataB->Travel[JumpDataA->Depth] = JumpDataA;
                JumpDataB->Radius=jmp->Radius;
                
                // Translation
                JumpDataB->VerseTo = vB;
                mat4x4_dup(JumpDataB->OffsetFrom, jmpCF);
                mat4x4_mul(JumpDataB->OffsetTo, JumpDataA->OffsetTo, (!k)? jmp->_ToV1 : jmp->_ToV0);
                
                // Validation
                vec4_dup(JumpDataB->Bounds, NewBounds);
                JumpDataB->Z_Bound = NewZDepth;
            }
        }
    }


    // Render verses from highest depth
    glEnable(GL_STENCIL_TEST);
    for (int i=stackpos; 0<i; i--) {
        JumpRenderData *rj = &VerseRenderStack[i];
        // Disable Z-buffer / Color buffer
        glColorMask(0,0,0,0);
        glDepthMask(0);

        // Increment stencil value by 1 on write if depth-test passes
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS,1,0xFFFFFFFF);
        glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
        // Render all portals needed to get to the currently drawn scene
        //renderJumpGeometry(rj,1); // Draw Portal Geometry
        // Set stencil buffer to read only
        // Allow test to pass only if its in space that intersects all drawn portals from previous step
        glStencilFunc(GL_EQUAL,rj->Depth,0xFFFFFFFF);
        glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
        // Re-enable Z-buffer / Color buffer
        glColorMask(1,1,1,1);
        glDepthMask(1);


        // Draw external scene
        setClipPlaneToJump(rj); // Set clipping plane to prevent external scene leaking into source scene
        renderVerse(window, &VerseRenderStack[i], t, step); // Draw Scene
        vec4_dup(ClipPlane, VEC4_ZERO); // Disable ClipPlane


        // Prevent source scenes from leaking into external scenes by shielding Z-Buffer with portal
        glColorMask(0,0,0,0);   // Disable color buffer
        //renderJumpGeometry(rj,0); // Draw Portal Goemetry
        glColorMask(1,1,1,1);   // Enable color buffer
    }
    glDisable(GL_STENCIL_TEST);
    renderVerse(window, &VerseRenderStack[0], t, step);
}


void OrderLights() {

}
void orderParts () {

}

void (mat4x4 ProjectionMatrix) {
    // Order stack
    // Ambient lights, all lights by group
    
    Enable_Blend();
    for (over_shader) {
        for (light_group) {
            order_using_if_in_light_group(over_shader==0? part->shader, over_shader);

            // Ordered stack
            for (light in group) {
                shader = over_shader==0? part->shader : over_shader;
                for shader {
                    for vbo {
                        for drawtype {
                            if new_shader
                                useProgram(shader)
                            bindVBO()

                            buildAtrributeList
                            drawArraysInstanced(drawType)
                        }
                    }
                }
            }
        }
    }

    glEnable(GL_BLEND);
    OrderLights()
    int overshdr=-1, lightgrp=-1;
    for (int l=0; l < MAX_STACK_LIGHTS; l++) {
        if ((overshdr!= Stack.Lights[l]->OverShader) || (lghtgrp!=Stack.Lights[l]->LightGroup)) {
            over_shader = Stack.Lights[l]->OverShader;
            lightgrp = lghtgrp!=Stack.Lights[l]->LightGroup;
            OrderParts(over_shader);
        }

        int counter; data d;
        int vao, drawmode, shader;
        for (int p=0; p<=MAX_STACK_PART; p++) {
            if (p==MAX_STACK_PART, vao != new_vao, drawmode != newdraw, shader !=newshader) {
                flush()
            }
            if (p==MAX_STACK_FLUSH)
            add_attributes(Size, CFrame)
        }
    }

    
} */




void flushRender(RenderStackData* RenderStack, mat4x4 ProjectionMatrix) {
    RenderStackData Ordered;


    getOrderedLights();
    getPartsByLight(lightgroup, overshader);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    int overshader=-1, lightgrp=-1;
    for (int i=0; i<MAX_VERSE_LIGHTS; i++) {
        LightInstance* l = Ordered.Lights[i].Instance;
        if ((l->OverShader!=overshader) || (l->LightGroup)) {
            
        }
    }
}



void pushOriginToRender(RenderStackData* RenderStack, OriginInstance* o, mat4x4 ViewMat) {
    // Parts
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        PartInstance* ch = o->Parts[i];
        if (ch==NULL) break;

        int n = RenderStack->NumParts;
        if (n==MAX_VERSE_PARTS) Error("RenderStack could not add part %s",ch->Name);
        RenderStack->NumParts++;

        // Add part
        mat4x4 ViewAt; 
        vec3 pivoff;
        for (int i=0; i<3; i++) pivoff[i] = ch->Pivot[i] * ch->Size[i];
        
        mat4x4_mul(ViewAt, ViewMat, ch->CFrame);
        vec3_add(ViewAt[3], ViewAt[3], pivoff);
        mat4x4_dup(RenderStack->Parts[n].ViewMat, ViewAt);
        RenderStack->Parts[n].Instance = ch;
    }


    // Lights
    for (int i=0; i<MAX_LIGHT_INSTANCES; i++) {
        LightInstance* ch = o->Lights[i];
        if (ch==NULL) break;

        int n = RenderStack->NumLights;
        if (n==MAX_VERSE_LIGHTS) Error("RenderStack could not add light %s",ch->Name);
        RenderStack->NumLights++;

        // Add light
        mat4x4_mul(RenderStack->Lights[n].ViewMat, ViewMat, ch->CFrame);
        RenderStack->Lights[n].Instance = ch;
    }


    // Origin Recursive
    for (int i=0; i<MAX_ORIGIN_INSTANCES; i++) {
        OriginInstance* ch = o->Origins[i];
        if (ch==NULL) break;
        
        mat4x4 ViewTo;
        mat4x4_mul(ViewTo, ViewMat, ch->CFrame);
        pushOriginToRender(RenderStack, ch, ViewTo);
    }
}




void renderStep(GLFWwindow* window, double t, double step) {
    JumpRenderData VerseStack[MAX_RENDER_VERSES] = {{
        .Depth = 0, 
        .Travel = {NULL},
        .Radius = 0,

        .VerseTo = NULL,

        .Bounds = {-1, 1, -1, 1},
        .Z_Bound = 0
    }};
    int stackpos=0;

    
    // Search verses
    // Start render stack from cameraview using camera
    GetCameraView(VerseStack[0].OffsetTo, &(VerseStack[0].VerseTo));
    mat4x4_invert(VerseStack[0].OffsetTo, VerseStack[0].OffsetTo);
    {
        // Build Verses
    }


    RenderStackData RenderStack = {
        .NumLights = 0,
        .NumParts = 0,
    };
    {
        // Loop Verses
    }
    //glDisable(GL_STENCIL_TEST);
    pushOriginToRender(&RenderStack, &(VerseStack[0].VerseTo->Root), VerseStack[0].OffsetTo);

    // Post Processing
}




void renderInit(GLFWwindow* window) {
    // Enable Face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_CLIP_DISTANCE0);
    
    // Build Verses
    SIMPLE_VERSE.Build(window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    SIMPLE_VERSE.Clean(window);
}