#version 410 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D atlas;

void main()
{ 
    fColor = texture(atlas, vTexCoord) + vec4(vColor, 1.0);
}
