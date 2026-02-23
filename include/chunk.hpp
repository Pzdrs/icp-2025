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
    BUILDING,
    READY,
    DIRTY,
    BUILT
};

using ChunkPosition = glm::ivec2;

class Chunk
{

public:
    static constexpr int SIZE_XZ = 16;
    static constexpr int SIZE_Y = 256;

    struct Mesh {
        std::vector<Vertex> solidVertices;
        std::vector<uint32_t> solidIndices;
    };

    Chunk(const ChunkManager &chunkManager, const ChunkPosition &position);
    ~Chunk();

    Mesh BuildMesh();
    void UploadMesh();
    void Draw(const Ref<Shader> &shader);

    void SetBlock(int x, int y, int z, Block::ID type) { blocks[x][y][z].type = type; }
    Block::State GetBlock(int x, int y, int z) const { return blocks[x][y][z]; }

    ChunkPosition GetPosition() const { return m_Position; }
    MeshState GetMeshState() const { return m_MeshState; }

    static glm::ivec2 GetChunkCoords(float worldX, float worldZ);

    void SetMeshState(MeshState state) { m_MeshState = state; }
    void SetMeshData(Mesh mesh);

private:
    bool IsFaceExposed(int x, int y, int z, Block::Face face, Block::ID type) const;

private:
    Block::State blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];

    MeshState m_MeshState = MeshState::DIRTY;

    std::vector<Vertex> m_SolidVertices;
    std::vector<uint32_t> m_SolidIndices;

    Ref<VertexArray> m_SolidVA;

    const ChunkPosition m_Position;
    const ChunkManager &m_ChunkManager;
};