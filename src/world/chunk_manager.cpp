#include "pch.hpp"
#include "world/chunk_manager.hpp"
#include "job_system.hpp"

void ChunkManager::ProcessCompletedJobs()
{
    {
        std::lock_guard lock(m_CompletedChunksMutex);
        while (!m_CompletedChunks.empty())
        {
            OnChunkGenerated(std::move(m_CompletedChunks.front()));
            m_CompletedChunks.pop();
        }
    }

    {
        std::lock_guard meshLock(m_CompletedMeshesMutex);
        while (!m_CompletedMeshes.empty())
        {
            auto [pos, mesh] = std::move(m_CompletedMeshes.front());
            OnMeshBuilt(pos, std::move(mesh));
            m_CompletedMeshes.pop();
        }
    }
}

void ChunkManager::EnsureChunkExists(ChunkPosition pos, const WorldGenerator &generator)
{
    {
        std::shared_lock lock(m_ChunksMutex);

        if (m_PendingChunks.contains(pos) || m_Chunks.contains(pos))
            return;
    }

    m_PendingChunks.insert(pos);
    JobSystem::Dispatch(
        [this, pos, &generator]()
        { GenerateChunk(pos, generator); });
}

void ChunkManager::UnloadChunk(ChunkPosition pos)
{
    std::unique_lock lock(m_ChunksMutex);

    m_Chunks.erase(pos);
    m_PendingChunks.erase(pos);

    for (auto [dir, offset] : std::vector<std::pair<Direction, ChunkPosition>>{
             {NORTH, {0, 1}}, {EAST, {1, 0}}, {SOUTH, {0, -1}}, {WEST, {-1, 0}}})
    {
        auto neighborIt = m_Chunks.find(pos + offset);
        if (neighborIt != m_Chunks.end())
        {
            neighborIt->second->SetMeshState(MeshState::DIRTY);
        }
    }
}

bool ChunkManager::HasChunk(ChunkPosition pos) const
{
    std::shared_lock lock(m_ChunksMutex);
    return m_Chunks.contains(pos);
}

Chunk *ChunkManager::GetChunk(int x, int z) const
{
    std::shared_lock lock(m_ChunksMutex);
    ChunkPosition pos(x, z);
    auto it = m_Chunks.find(pos);
    if (it != m_Chunks.end())
        return it->second.get();
    return nullptr;
}

void ChunkManager::OnChunkGenerated(Scope<Chunk> chunk)
{
    ChunkPosition pos = chunk->GetPosition();
    Chunk *chunkPtr = nullptr;

    {
        std::unique_lock lock(m_ChunksMutex);
        m_Chunks[pos] = std::move(chunk);
        m_PendingChunks.erase(pos);

        chunkPtr = m_Chunks[pos].get();
        chunkPtr->SetMeshState(MeshState::BUILDING);
    } // unlock before meshing

    JobSystem::Dispatch([this, pos, chunkPtr]()
                        {
        auto mesh = chunkPtr->BuildMesh();
        std::lock_guard lock(m_CompletedMeshesMutex);
        m_CompletedMeshes.push({pos, std::move(mesh)}); });

    // return;

    // for (auto [dir, offset] : std::vector<std::pair<Direction, ChunkPosition>>{
    //          {NORTH, {0, 1}}, {EAST, {1, 0}}, {SOUTH, {0, -1}}, {WEST, {-1, 0}}})
    // {
    //     auto neighborIt = m_Chunks.find(pos + offset);
    //     if (neighborIt != m_Chunks.end())
    //     {
    //         Chunk *neighborChunk = neighborIt->second.get();
    //         neighborChunk->SetMeshState(MeshState::BUILDING);
    //         JobSystem::Dispatch(
    //             [this, neighborChunk]()
    //             {
    //                 auto mesh = neighborChunk->BuildMesh();
    //                 std::lock_guard lock(m_CompletedMeshesMutex);
    //                 m_CompletedMeshes.push({neighborChunk->GetPosition(), std::move(mesh)});
    //             });
    //     }
    // }
}

void ChunkManager::OnMeshBuilt(ChunkPosition pos, Chunk::Mesh mesh)
{
    std::shared_lock lock(m_ChunksMutex);

    LOG("ChunkManager::OnMeshBuilt");
    auto it = m_Chunks.find(pos);
    if (it == m_Chunks.end())
        return;

    Chunk *chunk = it->second.get();

    // If chunk got invalidated while this mesh was building (e.g. neighbor arrived),
    // discard stale result and keep DIRTY state so it rebuilds with fresh neighbor data.
    if (chunk->GetMeshState() != MeshState::BUILDING)
        return;

    chunk->UploadMesh(mesh);
}

void ChunkManager::GenerateChunk(ChunkPosition pos, const WorldGenerator &generator)
{
    LOG("ChunkManager::GenerateChunk");
    auto newChunk = CreateScope<Chunk>(*this, pos);

    for (int x = 0; x < Chunk::SIZE_XZ; x++)
        for (int z = 0; z < Chunk::SIZE_XZ; z++)
        {
            int worldX = pos.x * Chunk::SIZE_XZ + x;
            int worldZ = pos.y * Chunk::SIZE_XZ + z;

            for (int y = 0; y < Chunk::SIZE_Y; y++)
            {
                newChunk->SetBlock(x, y, z, generator.GetBlock(worldX, y, worldZ).type);
            }
        }

    // sleep
    //  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::lock_guard lock(m_CompletedChunksMutex);
    m_CompletedChunks.push(std::move(newChunk));
}

void ChunkManager::BuildMesh(ChunkPosition pos)
{
    Chunk *chunk = nullptr;
    {
        std::shared_lock lockChunks(m_ChunksMutex);

        auto it = m_Chunks.find(pos);
        if (it == m_Chunks.end())
            return;

        if (it->second->GetMeshState() != MeshState::BUILDING)
            return;

        chunk = it->second.get();
    }

    auto mesh = chunk->BuildMesh();

    chunk->SetMeshState(MeshState::BUILT);

    std::lock_guard lock(m_CompletedMeshesMutex);
    m_CompletedMeshes.push({pos, std::move(mesh)});
}
