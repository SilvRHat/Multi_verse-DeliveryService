// Multi_verse DeliveryService
// Gavin Zimmerman
// Simple Vertex Node

#version 410 core

// INPUTS
// Vertex Attribs
layout(location = 0) in vec4 v_Vertex;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_UV;

// Reserved Instanced vertex attribs
layout(location = 3) in mat4 i_ModelViewMat;
layout(location = 7) in mat3 i_NormalMat;
layout(location =11) in vec4 i_Color;
layout(location =10) in vec3 i_Size;

// Node-special Instaced vertex attribs
layout(location =12) in vec2  n_TextureSize;
layout(location =13) in float n_Shiny;
layout(location =14) in float n_Emission;


// Uniforms
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;
uniform vec4 u_ClipPlane[6];
uniform vec2 u_Resolution;
uniform vec2 u_Mouse;
uniform float u_Time;

// Light Uniforms
uniform mat4 l_LightViewMat;




// OUTPUTS
layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec3 o_LightOffset;
layout(location = 2) out vec3 o_Norm;
layout(location = 3) out vec3 o_ViewPos;
layout(location = 4) out float o_Emission;
layout(location = 5) out float o_Shiny;
layout(location = 6) out vec2 o_UVMap;
layout(location = 7) out vec4 o_Vert;
layout(location = 9) out float o_Id;


out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[6];
};


// SOURCE
void main() {
    // Vertex Lighting Calculations
    vec4 vpos = i_ModelViewMat * (v_Vertex* vec4(i_Size,1));
    o_LightOffset = vec3(l_LightViewMat[3] - vpos);
    o_Norm = i_NormalMat * v_Normal;
    o_ViewPos = -vpos.xyz;
    
    o_Color = i_Color;
    o_Emission = n_Emission;
    o_Shiny = n_Shiny;
    o_UVMap = v_UV;
    o_Vert = v_Vertex * vec4(i_Size, 1);
    o_Id = float(gl_InstanceID);
    
    // Core Outputs
    //int i=0;
    //for (i=0; i<6; i++)
    //    gl_ClipDistance[i] = dot(i_ModelViewMat * v_Vertex, u_ClipPlane[i]);
    
    gl_Position = u_ProjectionMat * vpos;
}