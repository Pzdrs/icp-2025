#include "world.hpp"
#include <iostream>
#include "render/renderer.hpp"

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
    glm::ivec2 chunkPos(0, 0);
    auto newChunk = std::make_unique<Chunk>();

    for (int x = 0; x < Chunk::SIZE_XZ; x++)
        for (int y = 0; y < Chunk::SIZE_Y; y++)
            for (int z = 0; z < Chunk::SIZE_XZ; z++)
            {
                if (y < 4)
                    newChunk->blocks[x][y][z] = Block{blockRegistry.getID("stone")};
                else if (y < 8)
                    newChunk->blocks[x][y][z] = Block{blockRegistry.getID("dirt")};
                else if (y < 9)
                    newChunk->blocks[x][y][z] = Block{blockRegistry.getID("grass")};
                else
                    newChunk->blocks[x][y][z] = Block{blockRegistry.getID("air")};
            }

    newChunk->generateMesh(blockRegistry);

    chunks[chunkPos] = std::move(newChunk); // works fine
}