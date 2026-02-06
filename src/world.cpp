#include "world.hpp"
#include <iostream>
#include "render/renderer.hpp"
#include <glm/gtc/noise.hpp>

void World::Draw(const std::shared_ptr<Shader> &shader, const BlockRegistry &blockRegistry)
{
    for (auto &[pos, chunk] : m_Chunks)
    {
        chunk->Draw(shader);
    }
}

Chunk *World::GetChunk(int x, int z) const
{
    glm::ivec2 pos(x, z);
    auto it = m_Chunks.find(pos);
    if (it != m_Chunks.end())
        return it->second.get();
    return nullptr;
}

void World::Generate(const BlockRegistry &blockRegistry)
{
    std::cout << "Generating world...\n";

    for (int wx = 0; wx < WORLD_SIZE_XZ; wx++)
        for (int wz = 0; wz < WORLD_SIZE_XZ; wz++)
        {
            glm::ivec2 chunkPos(wx, wz);
            auto newChunk = std::make_unique<Chunk>(*this, chunkPos);

            for (int x = 0; x < Chunk::SIZE_XZ; x++)
                for (int z = 0; z < Chunk::SIZE_XZ; z++)
                {
                    // Compute world-space coordinates
                    int worldX = chunkPos.x * Chunk::SIZE_XZ + x;
                    int worldZ = chunkPos.y * Chunk::SIZE_XZ + z;

                    // Sample Perlin noise [-1,1]
                    float noise = glm::perlin(glm::vec2(worldX, worldZ) * 0.05f);

                    // Map noise to height
                    int baseHeight = 8;
                    int amplitude = 6;
                    int height = baseHeight + static_cast<int>(noise * amplitude);
                    height = glm::clamp(height, 1, Chunk::SIZE_Y - 1);

                    for (int y = 0; y < Chunk::SIZE_Y; y++)
                    {
                        if (y < height - 4)
                            newChunk->SetBlock(x, y, z, blockRegistry.getID("stone"));
                        else if (y < height - 1)
                            newChunk->SetBlock(x, y, z, blockRegistry.getID("dirt"));
                        else if (y == height - 1)
                            newChunk->SetBlock(x, y, z, blockRegistry.getID("grass"));
                        else
                            newChunk->SetBlock(x, y, z, blockRegistry.getID("air"));
                    }
                }

            newChunk->GenerateMesh(blockRegistry);
            m_Chunks[chunkPos] = std::move(newChunk);
        }
}
