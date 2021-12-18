// Fixed color GL4
#version 400 core

//  Input Color
uniform vec4 Color;
uniform mat3 NormalMatrix;
//  Fragment color
layout(location = 0) out vec4 OutDiffuseColor;

in vec3 N;

void main()
{ 
    vec3 X = NormalMatrix * vec3(1,1,1);
    float intensity = dot(normalize(N),normalize(X));

    float f;
    if (intensity > 0.95)
        f= 1.0;
    else if (intensity > 0.5)
        f = 0.6;
    else if (intensity > 0.25)
        f = 0.4;
    else
        f = 0.2;
    OutDiffuseColor = Color * f;
}
