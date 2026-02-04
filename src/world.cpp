#include "world.hpp"
#include <iostream>

void World::draw(const Renderer &renderer, const Shader &shader)
{
    for (auto &[pos, chunk] : chunks)
    {
        chunk->draw(renderer, shader);
    }
}

World::World(const BlockRegistry &blockRegistry)
{
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

    chunks[chunkPos] = std::move(newChunk);  // works fine
}