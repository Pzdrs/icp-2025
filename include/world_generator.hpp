#pragma once

#include "block.hpp"
#include "fast_noise_light.hpp"
#include "spline.hpp"

using GeneratorSeed = uint64_t;

class TerrainShaper
{
public:
    TerrainShaper(GeneratorSeed seed) : m_Seed(seed) {}
    virtual ~TerrainShaper() = default;
    virtual float GetHeight(int x, int z) const = 0;

protected:
    GeneratorSeed m_Seed;
};

class NoiseTerrainShaper : public TerrainShaper
{
    static constexpr std::pair<float, int> CONTINENTALNESS_SPLINE[] = {
        {-1.0f, 30},  // deep sea at ocean floor
        {-0.55f, 35}, // deep sea at ocean floor
        {-0.52f, 35}, // ocean floor lower bound
        {-0.5f, 40},  // ocean floor upper bound
        {-0.2f, 50},  // shallow water
        {-0.15f, 55}, // approaching shore
        {-0.1f, 60},  // near sea level
        {-0.07f, 63}, // gentle beach slope
        {-0.05f, 64}, // beach
        {-0.03f, 66}, // beach
        {-0.01f, 70}, // beach to land transition
        {0.0f, 74},   // land begins
        {0.05f, 77},  // gentle rise
        {0.1f, 80},   // full land elevation
        {0.3f, 120},  // higher land
        {1.0f, 150}   // mountains / high elevation
    };

public:
    NoiseTerrainShaper(GeneratorSeed seed) : TerrainShaper(seed)
    {
        InitNoise();
        InitSpline();
    }
    virtual ~NoiseTerrainShaper() = default;

    virtual float GetHeight(int x, int z) const override;

private:
    void InitNoise();
    void InitSpline();

private:
    FastNoiseLite m_ContinentalnessNoise, m_ErosionNoise, m_PVNoise;

    tk::spline m_ContinentalnessSpline, m_ErosionSpline, m_PVSpline;
};

class WorldGenerator
{
public:
    WorldGenerator(GeneratorSeed seed, BlockRegistry blockRegistry)
        : m_Seed(seed), m_BlockRegistry(blockRegistry) {}
    virtual ~WorldGenerator() = default;

    virtual Block GetBlock(int x, int y, int z) const = 0;

    inline GeneratorSeed GetSeed() const { return m_Seed; }
    inline BlockRegistry GetBlockRegistry() const { return m_BlockRegistry; }

protected:
    BlockRegistry m_BlockRegistry;
    GeneratorSeed m_Seed;
};

class OverworldGenerator : public WorldGenerator
{
public:
    OverworldGenerator(GeneratorSeed seed, BlockRegistry blockRegistry)
        : WorldGenerator(seed, blockRegistry), m_TerrainShaper(seed) {}
    virtual ~OverworldGenerator() = default;

    virtual Block GetBlock(int x, int y, int z) const override;

protected:
    int m_SeaLevel = 62;
    NoiseTerrainShaper m_TerrainShaper;
};