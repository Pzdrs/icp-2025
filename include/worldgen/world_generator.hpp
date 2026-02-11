#pragma once

#include "block.hpp"
#include "block_registry.hpp"

#include "terrain_shaper.hpp"
#include "surface_decorator.hpp"

class WorldGenerator
{
public:
    WorldGenerator(GeneratorSeed seed, BlockRegistry blockRegistry)
        : m_Seed(seed), m_BlockRegistry(blockRegistry) {}
    virtual ~WorldGenerator() = default;

    virtual Block::State GetBlock(int x, int y, int z) const = 0;

    inline GeneratorSeed GetSeed() const { return m_Seed; }
    inline BlockRegistry GetBlockRegistry() const { return m_BlockRegistry; }

protected:
    BlockRegistry m_BlockRegistry;
    GeneratorSeed m_Seed;
};

class OverworldGenerator : public WorldGenerator
{
public:
    OverworldGenerator(GeneratorSeed seed, Scope<TerrainShaper> terrainShaper, Scope<SurfaceDecorator> surfaceDecorator, BlockRegistry blockRegistry)
        : WorldGenerator(seed, blockRegistry), m_TerrainShaper(std::move(terrainShaper)), m_SurfaceDecorator(std::move(surfaceDecorator)) {}
    virtual ~OverworldGenerator() = default;

    virtual Block::State GetBlock(int x, int y, int z) const override;

protected:
    int m_SeaLevel = 62;
    Scope<TerrainShaper> m_TerrainShaper;
    Scope<SurfaceDecorator> m_SurfaceDecorator;
};