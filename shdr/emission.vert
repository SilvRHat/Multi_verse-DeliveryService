// Fixed color GL4
#version 400 core

//  Transformation matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

//  Vertex attributes (input)
layout (location=5) in vec4 Vertex;
layout (location=6) in vec3 Normal;

void main()
{
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}
