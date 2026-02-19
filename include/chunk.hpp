#pragma once
#include "render/vertex_array.hpp"
#include "render/shader.hpp"
#include "block.hpp"
#include "block_registry.hpp"
#include "render/buffer.hpp"
#include <glm/glm.hpp>
#include "direction.hpp"
#include "render/vertex.hpp"

class ChunkManager;

enum class MeshState
{
    NOT_READY = 0,
    READY,
    DIRTY
};

class Chunk
{

public:
    static constexpr int SIZE_XZ = 16;
    static constexpr int SIZE_Y = 256;

    Chunk(const ChunkManager &chunkManager, const glm::vec2 &worldPos);
    ~Chunk();

    void GenerateMesh();
    void Draw(const Ref<Shader> &shader);

    void NotifyNeighbor(Direction dir);

    void SetBlock(int x, int y, int z, Block::ID type) { blocks[x][y][z].type = type; }
    Block::State GetBlock(int x, int y, int z) const { return blocks[x][y][z]; }

    glm::ivec2 GetPosition() const { return glm::ivec2(m_WorldPos.x, m_WorldPos.y); }
    MeshState GetMeshState() const { return m_MeshState; }
    void MarkMeshDirty() { m_MeshState = MeshState::DIRTY; }

    static glm::ivec2 GetChunkCoords(float worldX, float worldZ);

private:
    bool IsFaceExposed(int x, int y, int z, Block::Face face, Block::ID type) const;

private:
    Block::State blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];
    MeshState m_MeshState = MeshState::NOT_READY;

    Ref<VertexArray> va = nullptr;

    const glm::vec2 m_WorldPos;
    const ChunkManager &m_ChunkManager;
};