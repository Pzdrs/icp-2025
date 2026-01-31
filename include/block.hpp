#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// using uint16 so chunks don't have to store a string id per block
using BlockID = uint16_t;

enum class Face : uint8_t
{
    FACE_BACK,
    FACE_FRONT,
    FACE_LEFT,
    FACE_RIGHT,
    FACE_UP,
    FACE_DOWN,
};

struct AtlasCoord
{
    unsigned int u;
    unsigned int v;
};

struct BlockTexture
{
    AtlasCoord top, side, bottom;

    bool isValid() const
    {
        return !(top.u == 0 && top.v == 0 &&
                 side.u == 0 && side.v == 0 &&
                 bottom.u == 0 && bottom.v == 0);
    }
};

struct Block
{
    BlockID type;
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

struct BlockDefinition
{
    std::string id, name;

    bool isSolid;
    float alpha;

    BlockTexture texture;
};

class BlockRegistry
{
public:
    BlockID registerBlock(const BlockDefinition &def);

    const BlockDefinition &get(BlockID id) const;

    const BlockDefinition &get(const std::string &stringID) const;

    BlockID getID(const std::string &stringID) const;

    size_t size() const { return blocks.size(); }

private:
    std::vector<BlockDefinition> blocks;
    std::unordered_map<std::string, BlockID> idLookup;
};

bool isSideFace(Face face);

BlockTexture getBlockTexture(const BlockID type, const BlockRegistry &blockRegistry);

BlockDefinition parseBlock(const json &j);

void loadBlockDefinitions(const std::string &manifestPath, BlockRegistry &registry);