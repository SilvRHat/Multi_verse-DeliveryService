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

layout(location = 9) in float i_Id;

uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

uniform vec4 l_Color;
uniform float u_Time;
uniform vec2 u_Resolution;


// OUTPUTS
layout (location=0) out vec4 o_Color;


// SOURCE
vec4 rand4(in vec4 p) {
    vec4 x = vec4(7.0, 57.0, 113.0, 607.0);
    return fract(sin(vec4(
            dot(p, x.xyzw),
            dot(p, x.ywzx), 
            dot(p, x.wzyx),
            dot(p, x.wyxz)
        )) *
    43758.5453);
}

void main() {
    o_Color = mix(i_Color, rand4(vec4(i_Id,0.,0.,0.)), .8);
}