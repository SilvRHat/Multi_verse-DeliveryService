// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"

// Globals
static vec4 ClipPlane = VEC4_ZERO;


void printmat(mat4x4 m) {
    for (int i=0; i<4; i++) { for (int j=0; j<4; j++)
        printf("%.2f ", m[i][j]); printf("\n");
    }
}

// SOURCE
void renderInstanceGeometry(PartInstance *p, mat4x4 ProjectMat, mat4x4 ViewMat) {
    unsigned int vao, program, loc;
    mat4x4 ModelViewMat, NormalMat4;
    float NormalMat[3][3];

    vao = p->Vao;
    program = p->Shader;
    if (!(vao && program && p->Vbo)) {
        printf("Cannot render geometry\n");
        return;
    }
    
    
    //  Bind VAO
    glUseProgram(program);
    glBindVertexArray(vao);
   
    //  Set Projection and Model View Matrix
    mat4x4_mul(ModelViewMat, ViewMat, p->CFrame);
    mat4x4_invert(NormalMat4, ModelViewMat);
    mat4x4_transpose(NormalMat4, NormalMat4);
    for (int i=0; i<3; i++) vec3_set(NormalMat[i],NormalMat4[i]);
    mat4x4_scale_aniso(ModelViewMat, ModelViewMat, p->Size[0], p->Size[1], p->Size[2]);

    

    loc = glGetUniformLocation(program,"ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ProjectMat);
    loc = glGetUniformLocation(program,"ModelViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ModelViewMat);
    loc = glGetUniformLocation(program,"NormalMatrix");
    glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat *)NormalMat);
    loc = glGetUniformLocation(program,"ViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ViewMat);

    //  Set color
    vec4 c = {0, 0, 0, p->Transparency};
    for (int i=0; i<3; i++) c[i] = p->Color[i]/255.0;
    loc = glGetUniformLocation(program, "Color");
    glUniform4fv(loc, 1, (const GLfloat *) c);

    loc = glGetUniformLocation(program, "ClipPlane");
    glUniform4fv(loc, 1, (const GLfloat *) ClipPlane);

    // Call instance drawing operations
    p->Draw(p);
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
    look[3] = vec3_magnitude(dist);

    // Flip Plane Normal and set Clipping Plane
    vec4_scale(ClipPlane, look, -1);
}


void renderVerse(GLFWwindow* window, JumpRenderData* jump, double t, double step) {
    VerseInstance* verse = jump->VerseTo;

    SignalFire(&verse->RenderStepped, window, t, step);
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        PartInstance* p = verse->Children[i];
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
                    vec2_set(BoundBox[3], (vec2){-rad, 0}); mat4x4_mul(lM, ToClipSpace, BoundBox);  // Left
                    vec2_set(BoundBox[3], (vec2){+rad, 0}); mat4x4_mul(rM, ToClipSpace, BoundBox);  // Right
                    vec2_set(BoundBox[3], (vec2){0, -rad}); mat4x4_mul(bM, ToClipSpace, BoundBox);  // Bottom
                    vec2_set(BoundBox[3], (vec2){0, +rad}); mat4x4_mul(tM, ToClipSpace, BoundBox);  // Top
                    
                    vec4 b0, b1; // Previous bounds, Current Bounds
                    vec4_set(b0, JumpDataA->Bounds);
                    // Set bounds in NDC-space; let each matrix have a different W-value
                    vec4_set(b1, (vec4){lM[3][0]/lM[3][3], rM[3][0]/rM[3][3], bM[3][1]/bM[3][3], tM[3][1]/tM[3][3]});
                    
                    // Bounds check
                    if (
                        (b0[0]>=b1[1]) || (b0[1]<=b1[0]) ||
                        (b0[2]>=b1[3]) || (b0[3]<=b1[2])
                    ) continue;
                    // Reconstrain bounds
                    vec4_set(NewBounds, (vec4){max(b0[0],b1[0]), min(b0[1],b1[1]), max(b0[2],b1[2]), min(b0[3],b1[3])});
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
                vec4_set(JumpDataB->Bounds, NewBounds);
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
        renderJumpGeometry(rj,1); // Draw Portal Geometry
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
        vec4_set(ClipPlane, VEC4_ZERO); // Disable ClipPlane


        // Prevent source scenes from leaking into external scenes by shielding Z-Buffer with portal
        glColorMask(0,0,0,0);   // Disable color buffer
        renderJumpGeometry(rj,0); // Draw Portal Goemetry
        glColorMask(1,1,1,1);   // Enable color buffer
    }
    glDisable(GL_STENCIL_TEST);
    renderVerse(window, &VerseRenderStack[0], t, step);
}


void renderInit(GLFWwindow* window) {
    // Enable Face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_CLIP_DISTANCE0);
    
    // Build Verses
    MULTI_VERSE.Build(&MULTI_VERSE, window);
    HOME_VERSE.Build(&HOME_VERSE, window);
    GATE_VERSE.Build(&GATE_VERSE, window);
    LIGHT_PLAY_VERSE.Build(&LIGHT_PLAY_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(&HOME_VERSE, window);
    MULTI_VERSE.Clean(&MULTI_VERSE, window);
    GATE_VERSE.Clean(&GATE_VERSE, window);
    LIGHT_PLAY_VERSE.Clean(&LIGHT_PLAY_VERSE, window);  
}