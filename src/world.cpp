#include "world.hpp"
#include <iostream>

void World::draw(const Renderer &renderer, const Shader &shader)
{
    for (auto &[pos, chunk] : chunks)
    {
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("uTransform", model);
        chunk->draw(renderer, shader);
    }
}

World::World()
{
    glm::ivec2 chunkPos(0, 0);
    auto newChunk = std::make_unique<Chunk>();

    for (int x = 0; x < Chunk::SIZE_XZ; x++)
        for (int y = 0; y < Chunk::SIZE_Y; y++)
            for (int z = 0; z < Chunk::SIZE_XZ; z++)
            {
                if (y < 4)
                    newChunk->blocks[x][y][z] = Block{BlockType::GRASS};
                else
                    newChunk->blocks[x][y][z] = Block{BlockType::AIR};
            }

    newChunk->generateMesh();

    chunks[chunkPos] = std::move(newChunk);  // works fine
}