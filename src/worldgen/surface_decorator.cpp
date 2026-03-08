#include "pch.hpp"
#include "worldgen/surface_decorator.hpp"

Scope<SurfaceDecorator> SurfaceDecorator::CreateOverworldDecorator(GeneratorSeed seed)
{
    return CreateScope<OverworldSurfaceDecorator>(seed);
}

std::optional<Block::State> OverworldSurfaceDecorator::GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const
{
    auto id = [](std::string name) { return BlockRegistry::Get().GetID(name); };

    if (y == surfaceHeight)
        return Block::State{ y < seaLevel - 1 ? id("gravel") : id("grass") };

    if (y < surfaceHeight && y >= surfaceHeight - 3)
        return Block::State{ id("dirt") };

    return std::nullopt;
}