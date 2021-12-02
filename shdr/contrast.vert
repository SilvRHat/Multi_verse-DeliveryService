// Fixed color GL4
#version 400 core

//  Transformation matrices
layout (location=3) uniform mat4 ModelViewMatrix;
layout (location=4) uniform mat4 ProjectionMatrix;

//  Vertex attributes (input)
layout (location=1) in vec4 Vertex;

void main()
{
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}
