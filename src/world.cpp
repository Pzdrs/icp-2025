#include "pch.hpp"
#include "world.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "render/renderer.hpp"
#include <glm/gtx/norm.hpp>
#include <unordered_set>
#include "job_system.hpp"

namespace std
{
    template <>
    struct hash<glm::ivec2>
    {
        size_t operator()(const glm::ivec2 &v) const noexcept
        {
            // 64-bit mix (very important for world coords)
            uint64_t x = static_cast<uint32_t>(v.x);
            uint64_t y = static_cast<uint32_t>(v.y);

            return (x << 32) ^ y;
        }
    };
}

void ChunkManager::ProcessCompletedJobs()
{
    std::lock_guard lock(m_CompletedMutex);

    while (!m_CompletedChunks.empty())
    {
        auto chunk = std::move(m_CompletedChunks.front());
        m_CompletedChunks.pop();

        glm::ivec2 pos = chunk->GetPosition();
        m_Chunks[pos] = std::move(chunk);
    }
}

void ChunkManager::EnsureChunkExists(glm::ivec2 pos, const WorldGenerator &generator)
{
    if (m_PendingChunks.contains(pos) || m_Chunks.contains(pos))
        return;

    m_PendingChunks.insert(pos);
    std::cout << "Requesting chunk at (" << pos.x << ", " << pos.y << ")\n";
    JobSystem::Dispatch(
        [this, pos, &generator]()
        { GenerateChunk(pos, generator); });
}

void ChunkManager::UnloadChunk(const glm::ivec2 &pos)
{
    std::cout << "Unloading chunk at (" << pos.x << ", " << pos.y << ")\n";
    m_Chunks.erase(pos);
    m_PendingChunks.erase(pos);
}

void World::Draw(const Ref<Shader> &shader, Ref<BlockRegistry> blockRegistry)
{
    for (auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        chunk->GenerateMesh(blockRegistry);
        chunk->Draw(shader);
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

void ChunkManager::GenerateChunk(const glm::ivec2 &pos, const WorldGenerator &generator)
{
    std::cout << "Generating chunk at (" << pos.x << ", " << pos.y << ")...\n";
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

    std::cout << "Finished generating chunk at (" << pos.x << ", " << pos.y << ")\n";

    {
        std::lock_guard lock(m_CompletedMutex);
        m_CompletedChunks.push(std::move(newChunk));
    }
}

void World::OnUpdate(const glm::vec3 &playerPos)
{
    // Determine which chunk the player is currently in
    glm::ivec2 playerChunk = glm::ivec2(
        floor(playerPos.x / Chunk::SIZE_XZ),
        floor(playerPos.z / Chunk::SIZE_XZ));

    // Only update if player has entered a new chunk
    if (playerChunk == m_LastPlayerChunk)
        return;

    m_LastPlayerChunk = playerChunk;

    // --- Collect required chunks ---
    std::vector<glm::ivec2> requiredChunks;

    for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
    {
        for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
        {
            glm::ivec2 pos = playerChunk + glm::ivec2(dx, dz);

            // Only request chunks not yet loaded
            if (!m_ChunkManager.HasChunk(pos))
                requiredChunks.push_back(pos);
        }
    }

    // --- Option 1: Sort by distance (nearest first) ---
    std::sort(requiredChunks.begin(), requiredChunks.end(),
              [playerChunk](const glm::ivec2 &a, const glm::ivec2 &b)
              {
                  auto da = glm::length2(glm::vec2(a - playerChunk));
                  auto db = glm::length2(glm::vec2(b - playerChunk));
                  return da < db;
              });

    // --- Request chunks in nearest-first order ---
    for (const auto &pos : requiredChunks)
        m_ChunkManager.EnsureChunkExists(pos, *m_Generator);

    // --- Unload chunks no longer required ---
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

void World::ProcessCompletedJobs()
{
    m_ChunkManager.ProcessCompletedJobs();
}