#include "world.hpp"
#include <iostream>
#include "render/renderer.hpp"
#include <glm/gtc/noise.hpp>

void World::draw(const std::shared_ptr<Shader> &shader)
{
    for (auto &[pos, chunk] : chunks)
    {
        chunk->draw(shader);
    }
}

void World::generate(const BlockRegistry &blockRegistry)
{
    std::cout << "Generating world...\n";

    for (int wx = 0; wx < WORLD_SIZE_XZ; wx++)
        for (int wz = 0; wz < WORLD_SIZE_XZ; wz++)
        {
            glm::ivec2 chunkPos(wx, wz);
            auto newChunk = std::make_unique<Chunk>(chunkPos);

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
                            newChunk->blocks[x][y][z] = Block{blockRegistry.getID("stone")};
                        else if (y < height - 1)
                            newChunk->blocks[x][y][z] = Block{blockRegistry.getID("dirt")};
                        else if (y == height - 1)
                            newChunk->blocks[x][y][z] = Block{blockRegistry.getID("grass")};
                        else
                            newChunk->blocks[x][y][z] = Block{blockRegistry.getID("air")};
                    }
                }

            newChunk->generateMesh(blockRegistry);
            chunks[chunkPos] = std::move(newChunk);
        }
}
