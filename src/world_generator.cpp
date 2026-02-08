#include "pch.hpp"
#include "world_generator.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include "chunk.hpp"

OverworldGenerator::~OverworldGenerator()
{
}

Block OverworldGenerator::GetBlock(int x, int y, int z) const
{
    // Sample Perlin noise [-1,1]
    float noise = glm::perlin(glm::vec2(x, z) * 0.05f);

    // Map noise to height
    int height = m_BaseHeight + static_cast<int>(noise * m_Amplitude);
    height = glm::clamp(height, 1, Chunk::SIZE_Y - 1);

    if (y < height - 4)
        return {m_BlockRegistry.getID("stone")};
    else if (y < height - 1)
        return {m_BlockRegistry.getID("dirt")};
    else if (y == height - 1)
        return {m_BlockRegistry.getID("grass")};
    else
        return {m_BlockRegistry.getID("air")};
}
