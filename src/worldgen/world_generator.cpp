#include "pch.hpp"
#include "worldgen/world_generator.hpp"
#include "chunk.hpp"
#include "block_registry.hpp"

Block::State OverworldGenerator::GetBlock(int x, int y, int z) const
{
    int surfaceY = m_TerrainShaper->GetHeight(x, z);

    if (y > surfaceY)
    {
        if (y < m_SeaLevel)
            return {BlockRegistry::Get().GetID("water")};
        else
            return {BlockRegistry::Get().GetID("air")};
    }

    auto decoratedBlock = m_SurfaceDecorator->GetBlock(x, y, z, surfaceY, m_SeaLevel);
    if (decoratedBlock)
        return *decoratedBlock;

    return {BlockRegistry::Get().GetID("stone")};
}