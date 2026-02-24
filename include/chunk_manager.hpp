#pragma once

#include "glm/glm.hpp"
#include "chunk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "worldgen/world_generator.hpp"
#include <shared_mutex>

using WorldPosition = glm::vec3;

struct ChunkPositionHash
{
    std::size_t operator()(const ChunkPosition &v) const noexcept
    {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);

        return h1 ^ (h2 << 1);
    }
};

class ChunkManager
{
public:
    const std::unordered_map<ChunkPosition, Scope<Chunk>, ChunkPositionHash> &GetChunks() const { return m_Chunks; }

    void ProcessCompletedJobs();

    // Ensures the chunk at the given position exists, dispatching a job to generate it if necessary.
    void EnsureChunkExists(ChunkPosition pos, const WorldGenerator &generator);
    void UnloadChunk(ChunkPosition pos);
    bool HasChunk(ChunkPosition pos) const;

    Chunk *GetChunk(int x, int z) const;

    void BuildMesh(ChunkPosition pos);

    void OnChunkGenerated(Scope<Chunk> chunk);
    void OnMeshBuilt(ChunkPosition pos, Chunk::Mesh mesh);

private:
    void GenerateChunk(ChunkPosition pos, const WorldGenerator &generator);

private:
    // Map of all loaded chunks, keyed by their chunk coordinates (not world coordinates)
    std::unordered_map<ChunkPosition, Scope<Chunk>, ChunkPositionHash> m_Chunks;

    // Set of chunk positions that are currently being generated but not yet completed (used to avoid duplicate generation)
    std::unordered_set<ChunkPosition, ChunkPositionHash> m_PendingChunks;

    // Queue of completed chunks waiting to be added to m_Chunks
    std::queue<Scope<Chunk>> m_CompletedChunks;
    // Queue of completed meshes waiting to be uploaded to their respective chunks
    std::queue<std::pair<ChunkPosition, Chunk::Mesh>> m_CompletedMeshes;

    std::mutex m_CompletedChunksMutex;
    std::mutex m_CompletedMeshesMutex;
    mutable std::shared_mutex m_ChunksMutex;
};
