// Multi_verse DeliveryService
// Gavin Zimmerman
// Simple Light Node

#version 410 core

// INPUTS
layout(location = 0) in vec4 i_Color;
layout(location = 1) in vec3 i_LightOffset;
layout(location = 2) in vec3 i_Norm;
layout(location = 3) in vec3 i_ViewPos;
layout(location = 4) in float i_Emission;
layout(location = 5) in float i_Shiny;
layout(location = 6) in vec2 i_UVMap;
layout(location = 7) in vec4 i_Vert;

uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;
uniform vec4 u_ClipPlane;
uniform vec2 u_Resolution;
uniform vec2 u_Mouse;
uniform float u_Time;

uniform int l_Enum;
uniform float l_Falloff;
uniform vec4 l_Color;
uniform vec4 l_Color1;
uniform float l_Radius;
uniform float l_Angle0;
uniform float l_Angle1;



// OUTPUTS
layout (location=0) out vec4 o_Color;


// SOURCE
void main() {
    float diffuseI = 0.; //max(dot(L, N), 0.);
    float specularI = 0.; //pow(max(dot(R, V), 0.), i_Specular)*.5;
    
    vec3 N,L,R,V;
    
        N = normalize(i_Norm);
        L = normalize(i_LightOffset);
        R = reflect(-L, N);
        V = normalize(i_ViewPos);

        diffuseI = max(dot(L, N), 0.);
        specularI = pow(max(dot(R, V), 0.), i_Shiny);

        // Point
        /*if (l_Radius>0) {
            float d = length(i_LightOffset);
            float att = max(0., 1.-(d*d/(l_Radius*l_Radius))); // Attenutation
            diffuseI *= att*att;
        }*/
        // Spot
    
    if (l_Enum==3) {
        // Ambient
        diffuseI = 1.;
    }

    vec4 st = vec4(i_UVMap,1,1);

    vec4 NDC = vec4(gl_FragCoord.xy / u_Resolution, gl_FragCoord.z, 1);

    o_Color = mix(NDC.rgba, i_Color, .8);//st;//(diffuseI * l_Color) + (specularI * l_Color1);
}