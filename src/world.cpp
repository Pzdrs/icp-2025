#include "pch.hpp"
#include "world.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

void World::Draw(const Ref<Shader> &shader)
{
    for (auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        if (chunk->GetMeshState() == MeshState::READY)
            chunk->Draw(shader);
    }
}

void World::OnUpdate(const glm::vec3 &playerPos)
{
    ChunkPosition playerChunk = Chunk::GetChunkCoords(playerPos.x, playerPos.z);
    if (playerChunk != m_LastPlayerChunk)
    {
        m_LastPlayerChunk = playerChunk;

        std::vector<ChunkPosition> requiredChunks;

        for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        {
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
            {
                ChunkPosition pos = playerChunk + ChunkPosition(dx, dz);

                if (!m_ChunkManager.HasChunk(pos))
                    requiredChunks.push_back(pos);
            }
        }

        std::sort(requiredChunks.begin(), requiredChunks.end(),
                  [playerChunk](const ChunkPosition &a, const ChunkPosition &b)
                  {
                      auto da = glm::length2(glm::vec2(a - playerChunk));
                      auto db = glm::length2(glm::vec2(b - playerChunk));
                      return da < db;
                  });

        for (const auto &pos : requiredChunks)
            m_ChunkManager.EnsureChunkExists(pos, *m_Generator);

        // UnloadFarChunks(playerChunk);
    }
}

void World::ProcessCompletedJobs()
{
    m_ChunkManager.ProcessCompletedJobs();
}

void World::UnloadFarChunks(ChunkPosition playerChunk)
{
    std::vector<ChunkPosition> toUnload;
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
