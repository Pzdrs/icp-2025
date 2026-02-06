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
    static constexpr int SIZE_XZ = 16;
    static constexpr int SIZE_Y = 256;

    Chunk(const World &world, const glm::ivec2 &worldPos);
    ~Chunk();

    void GenerateMesh(const BlockRegistry &blockRegistry);
    void Draw(const std::shared_ptr<Shader> &shader);

    void SetBlock(int x, int y, int z, BlockID type) { blocks[x][y][z].type = type; }
    Block GetBlock(int x, int y, int z) const { return blocks[x][y][z]; }

    static glm::ivec2 GetChunkCoords(float worldX, float worldZ);

private:
    bool IsFaceExposed(int x, int y, int z, int face, const BlockRegistry &blockRegistry) const;

private:
    Block blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];

    std::shared_ptr<VertexArray> va;

    const glm::ivec2 m_WorldPos;
    const World &m_World;
};
