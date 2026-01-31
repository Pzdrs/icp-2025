#pragma once

#include <glm/glm.hpp>

enum class BlockType : uint16_t
{
    AIR = 0,
    DIRT = 1,
    GRASS = 2,
    STONE = 3,
};

struct AtlasCoord
{
    unsigned int col;
    unsigned int row;
};

struct Block
{
    BlockType type;
};

// CUBE VERTICES
// --------------------------------

static const glm::vec3 CUBE_VERTS[6][4] = {
    // Back face (z = 0)
    {{0, 0, 0},   // back top left
     {1, 0, 0},   // back top right
     {1, -1, 0},  // back bottom right
     {0, -1, 0}}, // back bottom left

    // Front face (z = 1)
    {{0, 0, 1},
     {1, 0, 1},
     {1, -1, 1},
     {0, -1, 1}},

    // Left face (x = 0)
    {{0, 0, 1},
     {0, 0, 0},
     {0, -1, 0},
     {0, -1, 1}},

    // Right face (x = 1)
    {{1, 0, 0},
     {1, 0, 1},
     {1, -1, 1},
     {1, -1, 0}},

    // Top face (y = 0)
    {{0, 0, 0},
     {1, 0, 0},
     {1, 0, 1},
     {0, 0, 1}},

    // Bottom face (y = -1)
    {{0, -1, 1},
     {1, -1, 1},
     {1, -1, 0},
     {0, -1, 0}},
};

static const unsigned int FACE_INDICES[6] = {
    0, 1, 2,
    2, 3, 0};

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