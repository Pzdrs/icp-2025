#pragma once

#include <glm/glm.hpp>

#include "render/texture.hpp"
#include "render/sub_texture.hpp"

namespace Block
{
    using ID = uint16_t;

    struct State
    {
        ID type;
    };

    struct Texture
    {
        Ref<SubTexture2D> top, side, bottom;
    };

    enum class Face : uint8_t
    {
        FACE_BACK,
        FACE_FRONT,
        FACE_LEFT,
        FACE_RIGHT,
        FACE_UP,
        FACE_DOWN,
    };

    enum class TextureFace : uint8_t
    {
        SIDE,
        TOP,
        BOTTOM
    };

    // Returns the position of the vertex for the given face and vertex index (0-3)
    glm::vec3 GetVertexPosition(Face face, int vertex);
    // Returns the index of the vertex in the face's vertex array
    unsigned int GetFaceIndex(int index);
    // Converts a block face to the corresponding texture face (e.g. top face uses the block's top texture)
    TextureFace FaceToTextureFace(Face face);
    // Returns the direction vector for the given face (e.g. back face returns (0, 0, -1))
    glm::ivec3 GetFaceDirection(Face face);

}

// FACE TEXTURE COORDINATES
// --------------------------------

// TRIAL AND ERROR NEVIM
static const glm::vec2 FACE_UVS[4] = {
    {0, 1}, // top-left
    {1, 1}, // top-right
    {1, 0}, // bottom-right
    {0, 0}, // bottom-left
};

// TRIAL AND ERROR NEVIM
static const int FACE_UV_MAP[6][4] =
    {
        // -Z (back)
        {1, 0, 3, 2},
        // +Z (front)
        {0, 1, 2, 3},
        // -X (left)
        {1, 0, 3, 2},
        // +X (right)
        {1, 0, 3, 2},
        // +Y (top)
        {0, 1, 2, 3},
        // -Y (bottom)
        {0, 1, 2, 3}};
