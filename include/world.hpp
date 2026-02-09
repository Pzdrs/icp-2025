#pragma once

#include <glm/glm.hpp>
#include "chunk.hpp"
#include "world_generator.hpp"
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
    static constexpr int WORLD_SIZE_XZ = 2;

public:
    void Draw(const Ref<Shader> &shader, const BlockRegistry &blockRegistry);
    void Generate(const WorldGenerator &generator);

    Chunk *GetChunk(int x, int z) const;

private:
    std::unordered_map<glm::ivec2, Scope<Chunk>, IVec2Hash> m_Chunks;
};