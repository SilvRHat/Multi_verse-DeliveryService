// Fixed color GL4
#version 400 core

//  Input Color
uniform vec4  Color;
//  Fragment color
layout(location = 0) out vec4 OutDiffuseColor;

void main()
{
   OutDiffuseColor = Color;
}
