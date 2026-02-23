#include "pch.hpp"
#include "world.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "render/renderer.hpp"
#include <glm/gtx/norm.hpp>
#include <unordered_set>
#include "job_system.hpp"

void ChunkManager::ProcessCompletedJobs()
{
    {
        std::lock_guard lock(m_CompletedMutex);
        while (!m_CompletedChunks.empty())
        {
            OnChunkGenerated(std::move(m_CompletedChunks.front()));
            m_CompletedChunks.pop();
        }
    }

    {
        std::lock_guard meshLock(m_MeshMutex);
        while (!m_CompletedMeshes.empty())
        {
            auto [pos, mesh] = m_CompletedMeshes.front();
            OnMeshBuilt(pos, mesh);
            m_CompletedMeshes.pop();
        }
    }
}

void ChunkManager::EnsureChunkExists(glm::ivec2 pos, const WorldGenerator &generator)
{
    if (m_PendingChunks.contains(pos) || m_Chunks.contains(pos))
        return;

    m_PendingChunks.insert(pos);
    JobSystem::Dispatch(
        [this, pos, &generator]()
        { GenerateChunk(pos, generator); });
}

void ChunkManager::UnloadChunk(const glm::ivec2 &pos)
{
    m_Chunks.erase(pos);
    m_PendingChunks.erase(pos);

    for (auto [dir, offset] : std::vector<std::pair<Direction, glm::ivec2>>{
             {NORTH, {0, 1}}, {EAST, {1, 0}}, {SOUTH, {0, -1}}, {WEST, {-1, 0}}})
    {
        auto neighborIt = m_Chunks.find(pos + offset);
        if (neighborIt != m_Chunks.end())
        {
            neighborIt->second->SetMeshState(MeshState::DIRTY);
        }
    }
}

Chunk *ChunkManager::GetChunk(int x, int z) const
{
    glm::ivec2 pos(x, z);
    auto it = m_Chunks.find(pos);
    if (it != m_Chunks.end())
        return it->second.get();
    return nullptr;
}

void ChunkManager::OnChunkGenerated(Scope<Chunk> chunk)
{
    LOG("ChunkManager::OnChunkGenerated");
    glm::ivec2 pos = chunk->GetPosition();
    m_Chunks[pos] = std::move(chunk);
    m_PendingChunks.erase(pos);

    // m_Chunks[pos]->SetMeshState(MeshState::BUILDING);
    // // no need for a pending mesh check
    // LOG("Dispatch from ChunkManager::OnChunkGenerated");

    // JobSystem::Dispatch(
    //     [this, pos]()
    //     {
    //         auto it = m_Chunks.find(pos);
    //         if (it != m_Chunks.end())
    //         {
    //             auto mesh = it->second->BuildMesh();
    //             OnMeshBuilt(pos, mesh);
    //         }
    //     });

    for (auto [dir, offset] : std::vector<std::pair<Direction, glm::ivec2>>{
             {NORTH, {0, 1}}, {EAST, {1, 0}}, {SOUTH, {0, -1}}, {WEST, {-1, 0}}})
    {
        auto neighborIt = m_Chunks.find(pos + offset);
        if (neighborIt != m_Chunks.end())
        {
            neighborIt->second->SetMeshState(MeshState::DIRTY);
        }
    }
}

void ChunkManager::OnMeshBuilt(ChunkPosition pos, Chunk::Mesh mesh)
{
    LOG("ChunkManager::OnMeshBuilt");
    auto it = m_Chunks.find(pos);
    if (it != m_Chunks.end())
        it->second->SetMeshData(mesh);
}

void ChunkManager::GenerateChunk(const glm::ivec2 &pos, const WorldGenerator &generator)
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

    std::lock_guard lock(m_CompletedMutex);
    m_CompletedChunks.push(std::move(newChunk));
}

void World::Draw(const Ref<Shader> &shader)
{
    for (auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        // chunk->UploadMesh();
        if (chunk->GetMeshState() == MeshState::READY)
            chunk->Draw(shader);
    }
}

void World::OnUpdate(const glm::vec3 &playerPos)
{
    glm::ivec2 playerChunk = Chunk::GetChunkCoords(playerPos.x, playerPos.z);
    if (playerChunk != m_LastPlayerChunk)
    {
        m_LastPlayerChunk = playerChunk;

        std::vector<glm::ivec2> requiredChunks;

        for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        {
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
            {
                glm::ivec2 pos = playerChunk + glm::ivec2(dx, dz);

                if (!m_ChunkManager.HasChunk(pos))
                    requiredChunks.push_back(pos);
            }
        }

        std::sort(requiredChunks.begin(), requiredChunks.end(),
                  [playerChunk](const glm::ivec2 &a, const glm::ivec2 &b)
                  {
                      auto da = glm::length2(glm::vec2(a - playerChunk));
                      auto db = glm::length2(glm::vec2(b - playerChunk));
                      return da < db;
                  });

        for (const auto &pos : requiredChunks)
            m_ChunkManager.EnsureChunkExists(pos, *m_Generator);

        UnloadFarChunks(playerChunk);
    }

    for (auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        if (chunk->GetMeshState() == MeshState::DIRTY)
        {
            chunk->SetMeshState(MeshState::BUILDING);
            LOG("Dispatch from World::OnUpdate");
            JobSystem::Dispatch(
                [this, chunk = chunk.get()]()
                {
                    auto mesh = chunk->BuildMesh();
                    m_ChunkManager.OnMeshBuilt(chunk->GetPosition(), mesh);
                });

            return;
        }

        if (chunk->GetMeshState() == MeshState::BUILT)
        {
            chunk->UploadMesh();
        }
    }
}

void World::ProcessCompletedJobs()
{
    m_ChunkManager.ProcessCompletedJobs();
}

void World::UnloadFarChunks(const glm::ivec2 &playerChunk)
{
    std::vector<glm::ivec2> toUnload;
    for (const auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        int dx = pos.x - playerChunk.x;
        int dz = pos.y - playerChunk.y;

        if (std::abs(dx) > RENDER_DISTANCE || std::abs(dz) > RENDER_DISTANCE)
            toUnload.push_back(pos);
    }

    for (const auto &pos : toUnload)
        m_ChunkManager.UnloadChunk(pos);
}