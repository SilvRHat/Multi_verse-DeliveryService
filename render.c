// Multi_verse DeliveryService
// Gavin Zimmerman

// Render - World Controller
#include "render.h"
static vec4 ClipPlane = VEC4_ZERO;


// SOURCE
void renderInstanceGeometry(PartInstance *p, mat4x4 ProjectMat, mat4x4 ViewMat) {
    unsigned int vao, program, loc;
    mat4x4 ModelViewMat;

    vao = p->Vao;
    program = p->Shader;
    if (!(vao && program && p->Vbo))
        return;
    
    //  Bind VAO
    glUseProgram(program);
    glBindVertexArray(vao);
   
    //  Set Projection and Model View Matrix
    mat4x4_mul(ModelViewMat, ViewMat, p->CFrame);
    mat4x4_scale_aniso(ModelViewMat, ModelViewMat, p->Size[0], p->Size[1], p->Size[2]);

    loc = glGetUniformLocation(program,"ProjectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ProjectMat);
    loc = glGetUniformLocation(program,"ModelViewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat *)ModelViewMat);

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


void renderJumpGeometry(JumpRenderData* rj) {
    // Draw Circle 
    PartInstance *portal = circle(INST_HI_SEGMTS, rj->Jump->Radius); {
        mat4x4 CF;
        mat4x4_identity(CF);
        mat4x4_rotate_X(CF, CF, 90*TO_RAD);
        PartSetShader(portal, EMISSION_SHDR);
        PartSetCFrame(portal, CF);
    }
    renderInstanceGeometry(portal, rj->VerseTo->ProjectMatrix, rj->OffsetFrom);
    DestroyPartInstance(portal);
}


void renderVerse(JumpRenderData* jump, double t, double step) {
    VerseInstance* verse = jump->VerseTo;

    SignalFire(&verse->RenderStepped, t, step);
    for (int i=0; i<MAX_PART_INSTANCES; i++) {
        PartInstance* p = verse->Children[i];
        if (p==NULL)
            break;
        // Perform Object Culling [TODO]

        // Draw Instance
        renderInstanceGeometry(p, verse->ProjectMatrix, jump->OffsetTo);
    }
}


void renderStep(GLFWwindow* window, double t, double step) {
    JumpRenderData VerseRenderStack[MAX_RENDER_VERSES] = {{
        .Depth = 0, 
        .VerseTo = NULL,
        .Bounds = {-1, 1, -1, 1},
        .Z_Bound = 0,
        .Jump = NULL
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

                JumpDataB->Depth = JumpDataA->Depth+1;
                JumpDataB->VerseTo = vB;
                mat4x4_dup(JumpDataB->OffsetFrom, jmpCF);
                mat4x4_mul(JumpDataB->OffsetTo, JumpDataA->OffsetTo, (!k)? jmp->_ToV1 : jmp->_ToV0);
                vec4_set(JumpDataB->Bounds, NewBounds);
                JumpDataB->Z_Bound = NewZDepth;
                JumpDataB->Jump=jmp;
            }
        }
    }


    // Render verses from highest depth
    for (int i=stackpos; 0<i; i--) {
        JumpRenderData *rj = &VerseRenderStack[i];
        // Disable Z-buffer / Color buffer
        // Enable Stencil buffer
        renderJumpGeometry(rj); // Draw Portal Geometry
        
        // Enable Z-buffer / Color buffer
        // Set clip plane
        renderVerse(&VerseRenderStack[i], t, step); // Draw Scene

        // Disable color buffer
        // renderJumpGeometry(rj); // Draw Portal Goemetry
    }
    // Enable color buffer
    renderVerse(&VerseRenderStack[0], t, step);
}




void renderInit(GLFWwindow* window) {
    // Enable Face culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_CLIP_DISTANCE0);
    glCullFace(GL_FRONT);
    // Build Verses
    MULTI_VERSE.Build(&MULTI_VERSE, window);
    HOME_VERSE.Build(&HOME_VERSE, window);
    GATE_VERSE.Build(&GATE_VERSE, window);
}


void renderExit(GLFWwindow* window) {
    // Deconstruct Verses
    HOME_VERSE.Clean(&HOME_VERSE, window);
    MULTI_VERSE.Clean(&MULTI_VERSE, window);
    GATE_VERSE.Clean(&GATE_VERSE, window);  
}