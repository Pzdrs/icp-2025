#version 410 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D uTexture;

void main()
{ 
    fColor = texture(uTexture, vTexCoord) + vec4(vColor, 1.0);
}
