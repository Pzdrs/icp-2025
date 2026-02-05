#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

uniform mat4 uTransform;
uniform mat4 uViewProjection;

void main()
{
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
}