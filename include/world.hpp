#pragma once

#include <glm/glm.hpp>
#include "chunk.hpp"
#include "worldgen/world_generator.hpp"
#include "block.hpp"
#include <unordered_set>
#include <unordered_map>

using WorldPosition = glm::vec3;

struct IVec2Hash
{
    std::size_t operator()(const glm::ivec2 &v) const noexcept
    {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);

        return h1 ^ (h2 << 1);
    }
};

class ChunkManager
{
public:
    const std::unordered_map<ChunkPosition, Scope<Chunk>, IVec2Hash> &GetChunks() const { return m_Chunks; }

    void ProcessCompletedJobs();

    void EnsureChunkExists(glm::ivec2 pos, const WorldGenerator &generator);
    void UnloadChunk(const glm::ivec2 &pos);
    bool HasChunk(const glm::ivec2 &pos) const { return m_Chunks.contains(pos) || m_PendingChunks.contains(pos); }

    Chunk *GetChunk(int x, int z) const;

    void OnChunkGenerated(Scope<Chunk> chunk);
    void OnMeshBuilt(ChunkPosition pos, Chunk::Mesh mesh);

private:
    void GenerateChunk(const glm::ivec2 &pos, const WorldGenerator &generator);

private:
    std::unordered_map<ChunkPosition, Scope<Chunk>, IVec2Hash> m_Chunks;

    std::unordered_set<ChunkPosition, IVec2Hash> m_PendingChunks;

    std::queue<Scope<Chunk>> m_CompletedChunks;
    std::queue<std::pair<ChunkPosition, Chunk::Mesh>> m_CompletedMeshes;

    std::mutex m_CompletedMutex;
    std::mutex m_MeshMutex;
};

class World
{
    static constexpr int RENDER_DISTANCE = 1;

public:
    World(Scope<WorldGenerator> generator) : m_Generator(std::move(generator)) {}
    ~World() = default;

    void Draw(const Ref<Shader> &shader);
    void OnUpdate(const glm::vec3 &playerPos);
    void ProcessCompletedJobs();

    ChunkManager &GetChunkManager() { return m_ChunkManager; }

private:
    void UnloadFarChunks(const glm::ivec2 &playerChunk);

private:
    Scope<WorldGenerator> m_Generator;
    ChunkManager m_ChunkManager;

    glm::ivec2 m_LastPlayerChunk = {INT32_MIN, INT32_MIN};
};