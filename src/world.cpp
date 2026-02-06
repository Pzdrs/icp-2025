#include "world.hpp"
#include <iostream>
#include "render/renderer.hpp"
#include <glm/gtc/noise.hpp>
#include <vector>
#include <cmath>

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

bool World::InBoundsXZ(int worldX, int worldZ) const
{
    (void)worldX;
    (void)worldZ;
    return true;
}

int World::FloorDiv(int a, int b)
{
    int q = a / b;
    int r = a % b;
    if (r != 0 && ((r < 0) != (b < 0)))
        q--;
    return q;
}

BlockID World::GetBlockID(int worldX, int worldY, int worldZ, const BlockRegistry &blockRegistry) const
{
    if (!InBoundsXZ(worldX, worldZ))
        return blockRegistry.getID("air");

    if (worldY < 0 || worldY >= Chunk::SIZE_Y)
        return blockRegistry.getID("air");

    const int chunkX = FloorDiv(worldX, Chunk::SIZE_XZ);
    const int chunkZ = FloorDiv(worldZ, Chunk::SIZE_XZ);

    const int localX = worldX - chunkX * Chunk::SIZE_XZ;
    const int localZ = worldZ - chunkZ * Chunk::SIZE_XZ;

    Chunk *chunk = GetChunk(chunkX, chunkZ);
    if (!chunk)
        return blockRegistry.getID("air");

    return chunk->GetBlock(localX, worldY, localZ).type;
}

bool World::IsSolid(int worldX, int worldY, int worldZ, const BlockRegistry &blockRegistry) const
{
    BlockID id = GetBlockID(worldX, worldY, worldZ, blockRegistry);
    const BlockDefinition &def = blockRegistry.get(id);
    return def.isSolid;
}

bool World::SetBlock(int worldX, int worldY, int worldZ, BlockID type, const BlockRegistry &blockRegistry)
{
    if (!InBoundsXZ(worldX, worldZ))
        return false;

    if (worldY < 0 || worldY >= Chunk::SIZE_Y)
        return false;

    const int chunkX = FloorDiv(worldX, Chunk::SIZE_XZ);
    const int chunkZ = FloorDiv(worldZ, Chunk::SIZE_XZ);

    const int localX = worldX - chunkX * Chunk::SIZE_XZ;
    const int localZ = worldZ - chunkZ * Chunk::SIZE_XZ;

    Chunk *chunk = GetChunk(chunkX, chunkZ);
    if (!chunk)
        return false;

    chunk->SetBlock(localX, worldY, localZ, type);
    chunk->GenerateMesh(blockRegistry);

    // Update neighbors if we touched a border.
    if (localX == 0)
        if (Chunk *left = GetChunk(chunkX - 1, chunkZ))
            left->GenerateMesh(blockRegistry);
    if (localX == Chunk::SIZE_XZ - 1)
        if (Chunk *right = GetChunk(chunkX + 1, chunkZ))
            right->GenerateMesh(blockRegistry);
    if (localZ == 0)
        if (Chunk *back = GetChunk(chunkX, chunkZ - 1))
            back->GenerateMesh(blockRegistry);
    if (localZ == Chunk::SIZE_XZ - 1)
        if (Chunk *front = GetChunk(chunkX, chunkZ + 1))
            front->GenerateMesh(blockRegistry);

    return true;
}

void World::Generate(const BlockRegistry &blockRegistry)
{
    std::cout << "Generating initial chunks...\n";
    EnsureChunk(0, 0, blockRegistry);
}

void World::Update(const BlockRegistry &blockRegistry, const glm::vec3 &playerPos, int viewDistance)
{
    const int playerChunkX = FloorDiv(static_cast<int>(std::floor(playerPos.x)), Chunk::SIZE_XZ);
    const int playerChunkZ = FloorDiv(static_cast<int>(std::floor(playerPos.z)), Chunk::SIZE_XZ);

    for (int dz = -viewDistance; dz <= viewDistance; dz++)
        for (int dx = -viewDistance; dx <= viewDistance; dx++)
        {
            const int cx = playerChunkX + dx;
            const int cz = playerChunkZ + dz;
            EnsureChunk(cx, cz, blockRegistry);
        }

    // Unload far chunks
    std::vector<glm::ivec2> toRemove;
    for (auto &entry : m_Chunks)
    {
        const glm::ivec2 &pos = entry.first;
        const int distX = std::abs(pos.x - playerChunkX);
        const int distZ = std::abs(pos.y - playerChunkZ);
        if (distX > viewDistance + 1 || distZ > viewDistance + 1)
            toRemove.push_back(pos);
    }

    for (const auto &pos : toRemove)
        m_Chunks.erase(pos);
}

void World::EnsureChunk(int chunkX, int chunkZ, const BlockRegistry &blockRegistry)
{
    glm::ivec2 pos(chunkX, chunkZ);
    if (m_Chunks.find(pos) != m_Chunks.end())
        return;

    GenerateChunk(pos, blockRegistry);
}

void World::GenerateChunk(const glm::ivec2 &chunkPos, const BlockRegistry &blockRegistry)
{
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
