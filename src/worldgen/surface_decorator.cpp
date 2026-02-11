#include "pch.hpp"
#include "worldgen/surface_decorator.hpp"

Scope<SurfaceDecorator> SurfaceDecorator::CreateOverworldDecorator(GeneratorSeed seed, BlockRegistry &blockRegistry)
{
    return CreateScope<OverworldSurfaceDecorator>(seed, blockRegistry);
}

const Block::State *OverworldSurfaceDecorator::GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const
{
    static const Block::State GRASS{m_BlockRegistry.GetID("grass")};
    static const Block::State DIRT{m_BlockRegistry.GetID("dirt")};
    static const Block::State GRAVEL{m_BlockRegistry.GetID("gravel")};

    if (y == surfaceHeight)
    {
        if (y < seaLevel - 1)
            return &GRAVEL;
        else
            return &GRASS;
    }
    else if (y < surfaceHeight && y >= surfaceHeight - 3)
    {
        return &DIRT;
    }

    return nullptr;
}