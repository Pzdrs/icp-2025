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
    void draw(const Shader &shader);
    void generate(const BlockRegistry &blockRegistry);

private:
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, IVec2Hash> chunks;
};