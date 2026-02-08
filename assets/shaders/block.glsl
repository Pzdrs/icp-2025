#type vertex
#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec4 vColor;
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

in vec4 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D uTexture;

void main()
{ 
    vec4 texColor = texture(uTexture, vTexCoord);

    vec3 finalRGB = texColor.rgb + vColor.rgb; // additive tint
    float finalA = texColor.a * vColor.a;      // keep transparency
    fColor = vec4(finalRGB, finalA);
}
