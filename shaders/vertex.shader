#version 410 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

void main()
{
    gl_Position = aPos;
    vColor = aColor;
    vTexCoord = aTexCoord;
}