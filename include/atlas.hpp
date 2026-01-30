#pragma once

#include <iostream>
#include "stb_image.h"
#include <glad/glad.h>

struct UVRect
{
    float u0, v0; // bottom-left
    float u1, v1; // top-right
};

// Get UV coordinates for a sprite at (col, row)
inline UVRect getSpriteUV(int col, int row, int spriteSize, int atlasWidth, int atlasHeight)
{
    float uSize = (float)spriteSize / atlasWidth;
    float vSize = (float)spriteSize / atlasHeight;

    // OpenGL UV origin is bottom-left
    float u0 = col * uSize;
    float v0 = row * vSize;
    float u1 = u0 + uSize;
    float v1 = v0 + vSize;

    return {u0, v0, u1, v1};
}

inline unsigned int initAtlas(const char *path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded texture: " << path << " (" << width << "x" << height << ", " << nrChannels << " channels)" << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}
