#pragma once

#include "block.hpp"
#include <block_registry.hpp>

class SurfaceDecorator
{
public:
    SurfaceDecorator(GeneratorSeed seed) : m_Seed(seed) {}
    virtual ~SurfaceDecorator() = default;

    virtual const Block::State *GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const = 0;

    static Scope<SurfaceDecorator> CreateOverworldDecorator(GeneratorSeed seed);

protected:
    GeneratorSeed m_Seed;
};

class OverworldSurfaceDecorator : public SurfaceDecorator
{
public:
    OverworldSurfaceDecorator(GeneratorSeed seed) : SurfaceDecorator(seed) {}
    virtual ~OverworldSurfaceDecorator() = default;

    virtual const Block::State *GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const override;
};