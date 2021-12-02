// Fixed color GL4
#version 400 core

//  Input Color
uniform vec4  Kd;
in vec3 normal;
in vec3 light;
//  Fragment color
layout (location=0) out vec4 FragColor;

void main()
{
    float edge = Dot(normal, light);
    if (edge>.9)
        FragColor = vec4(1,1,1,1);
    else if (edge<.1)
        FragColor = vec4(1,1,1,2) - Kd;
    else
        FragColor = Kd;
}
