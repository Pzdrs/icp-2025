#include "pch.hpp"
#include "worldgen/surface_decorator.hpp"

Scope<SurfaceDecorator> SurfaceDecorator::CreateOverworldDecorator(GeneratorSeed seed)
{
    return CreateScope<OverworldSurfaceDecorator>(seed);
}

const Block::State *OverworldSurfaceDecorator::GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const
{
    static const Block::State GRASS{BlockRegistry::Get().GetID("grass")};
    static const Block::State DIRT{BlockRegistry::Get().GetID("dirt")};
    static const Block::State GRAVEL{BlockRegistry::Get().GetID("gravel")};

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