#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

//output the color for the fragment shader
out vec3 color;

out vec2 texCoord;

//uniform float scale;

//the transformation matrix
uniform mat4 transform;

void main()
{
   gl_Position = transform * vec4(aPos, 1.0);
   //assigns the color from the vertex data to "color"
   color = aColor;
   texCoord = aTex;
}