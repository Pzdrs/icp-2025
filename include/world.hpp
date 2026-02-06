#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "chunk.hpp"
#include <iostream>
#include "block.hpp"

struct IVec2Hash
{
    std::size_t operator()(const glm::ivec2 &v) const noexcept
    {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);

        return h1 ^ (h2 << 1);
    }
};

class World
{
public:
    void Draw(const std::shared_ptr<Shader> &shader, const BlockRegistry &blockRegistry);
    void Generate(const BlockRegistry &blockRegistry);
    void Update(const BlockRegistry &blockRegistry, const glm::vec3 &playerPos, int viewDistance);

    Chunk *GetChunk(int x, int z) const;

    BlockID GetBlockID(int worldX, int worldY, int worldZ, const BlockRegistry &blockRegistry) const;
    bool IsSolid(int worldX, int worldY, int worldZ, const BlockRegistry &blockRegistry) const;
    bool SetBlock(int worldX, int worldY, int worldZ, BlockID type, const BlockRegistry &blockRegistry);

    bool InBoundsXZ(int worldX, int worldZ) const;

private:
    void EnsureChunk(int chunkX, int chunkZ, const BlockRegistry &blockRegistry);
    void GenerateChunk(const glm::ivec2 &chunkPos, const BlockRegistry &blockRegistry);
    static int FloorDiv(int a, int b);

    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> m_Chunks;
};
