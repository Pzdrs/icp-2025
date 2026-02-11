#include "pch.hpp"
#include "world.hpp"

#include "render/renderer.hpp"
#include <glm/gtc/noise.hpp>

void World::Draw(const Ref<Shader> &shader, const BlockRegistry &blockRegistry)
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

void World::Generate(const WorldGenerator &generator)
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
                    int worldX = chunkPos.x * Chunk::SIZE_XZ + x;
                    int worldZ = chunkPos.y * Chunk::SIZE_XZ + z;

                    for (int y = 0; y < Chunk::SIZE_Y; y++)
                    {
                        newChunk->SetBlock(x, y, z, generator.GetBlock(worldX, y, worldZ).type);
                    }
                }

            newChunk->GenerateMesh(generator.GetBlockRegistry());
            m_Chunks[chunkPos] = std::move(newChunk);
        }
}
