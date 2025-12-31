#version 430 core
out vec4 FragColor;

//inputs the color from the vertex shader
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
   //FragColor = vec4(color, 1.0f);
   FragColor = texture(tex0, texCoord);
}