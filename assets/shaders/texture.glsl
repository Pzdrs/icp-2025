#type vertex
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

#type fragment
#version 410 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D uTexture;

void main()
{ 
    fColor = texture(uTexture, vTexCoord) + vec4(vColor, 1.0);
}
