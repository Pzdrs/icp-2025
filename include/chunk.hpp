#pragma once
#include "render/vertex_array.hpp"
#include "render/renderer.hpp"
#include "block.hpp"
#include "render/buffer.hpp"
#include <glm/glm.hpp>

class World;

class Chunk
{

public:
    static constexpr int SIZE_XZ = 1;
    static constexpr int SIZE_Y = 1;

    Chunk(const World &world, const glm::vec2 &worldPos);
    ~Chunk();

    void GenerateMesh(const BlockRegistry &blockRegistry);
    void Draw(const Ref<Shader> &shader);

    void SetBlock(int x, int y, int z, BlockID type) { blocks[x][y][z].type = type; }
    Block GetBlock(int x, int y, int z) const { return blocks[x][y][z]; }

    static glm::ivec2 GetChunkCoords(float worldX, float worldZ);

private:
    bool IsFaceExposed(int x, int y, int z, int face, BlockID type, const BlockRegistry &blockRegistry) const;

private:
    Block blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];

    Ref<VertexArray> va;

    const glm::vec2 m_WorldPos;
    const World &m_World;
};