#include "pch.hpp"
#include "block.hpp"

using namespace Block;

static const glm::vec3 CUBE_VERTS[6][4] = {
    // Back face (z = 0)
    {{1, 0, 0},   // back top left
     {0, 0, 0},   // back top right
     {0, -1, 0},  // back bottom right
     {1, -1, 0}}, // back bottom left

    // Front face (z = 1)
    {{0, 0, 1},
     {1, 0, 1},
     {1, -1, 1},
     {0, -1, 1}},

    // Left face (x = 0)
    {{0, 0, 0},
     {0, 0, 1},
     {0, -1, 1},
     {0, -1, 0}},

    // Right face (x = 1)
    {{1, 0, 1},
     {1, 0, 0},
     {1, -1, 0},
     {1, -1, 1}},

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

// CW winding
static const unsigned int FACE_INDICES[6] = {
    0, 1, 2,
    2, 3, 0};

// tied to the order of faces in the CUBE_VERTS array
static const glm::ivec3 FACE_DIRS[6] = {
    {0, 0, -1}, // -Z
    {0, 0, 1},  // +Z
    {-1, 0, 0}, // -X
    {1, 0, 0},  // +X
    {0, 1, 0},  // +Y
    {0, -1, 0}  // -Y
};

const std::unordered_map<Face, TextureFace> FACE_TO_TEXTURE_FACE = {
    {Face::FACE_BACK, TextureFace::SIDE},
    {Face::FACE_FRONT, TextureFace::SIDE},
    {Face::FACE_LEFT, TextureFace::SIDE},
    {Face::FACE_RIGHT, TextureFace::SIDE},
    {Face::FACE_UP, TextureFace::TOP},
    {Face::FACE_DOWN, TextureFace::BOTTOM},
};

glm::vec3 Block::GetVertexPosition(Face face, int vertex)
{
    return CUBE_VERTS[static_cast<int>(face)][vertex];
}

unsigned int Block::GetFaceIndex(int index)
{
    return FACE_INDICES[index];
}

Block::TextureFace Block::FaceToTextureFace(Face face)
{
    return FACE_TO_TEXTURE_FACE.at(face);
}

glm::ivec3 Block::GetFaceDirection(Face face)
{
    return FACE_DIRS[static_cast<int>(face)];
}
