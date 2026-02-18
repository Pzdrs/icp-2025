#include "pch.hpp"
#include "worldgen/world_generator.hpp"
#include "chunk.hpp"

Block::State OverworldGenerator::GetBlock(int x, int y, int z) const
{
    int surfaceY = m_TerrainShaper->GetHeight(x, z);

    if (y > surfaceY)
    {
        if (y < m_SeaLevel)
            return {m_BlockRegistry->GetID("water")};
        else
            return {m_BlockRegistry->GetID("air")};
    }

    auto decoratedBlock = m_SurfaceDecorator->GetBlock(x, y, z, surfaceY, m_SeaLevel);
    if (decoratedBlock)
        return *decoratedBlock;

    return {m_BlockRegistry->GetID("stone")};
}