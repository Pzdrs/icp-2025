#pragma once

#include "block.hpp"
#include <block_registry.hpp>

class SurfaceDecorator
{
public:
    SurfaceDecorator(GeneratorSeed seed, BlockRegistry &blockRegistry) : m_Seed(seed), m_BlockRegistry(blockRegistry) {}
    virtual ~SurfaceDecorator() = default;

    virtual const Block::State *GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const = 0;

    static Scope<SurfaceDecorator> CreateOverworldDecorator(GeneratorSeed seed, BlockRegistry &blockRegistry);

protected:
    GeneratorSeed m_Seed;
    BlockRegistry &m_BlockRegistry;
};

class OverworldSurfaceDecorator : public SurfaceDecorator
{
public:
    OverworldSurfaceDecorator(GeneratorSeed seed, BlockRegistry &blockRegistry) : SurfaceDecorator(seed, blockRegistry) {}
    virtual ~OverworldSurfaceDecorator() = default;

    virtual const Block::State *GetBlock(int x, int y, int z, int surfaceHeight, int seaLevel) const override;
};