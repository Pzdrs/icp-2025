#pragma once
#include "fast_noise_light.hpp"
#include "spline.hpp"
#include <glm/gtc/noise.hpp>

class TerrainShaper
{
public:
    TerrainShaper(GeneratorSeed seed) : m_Seed(seed) {}
    virtual ~TerrainShaper() = default;

    virtual float GetHeight(int x, int z) const = 0;

    static Scope<TerrainShaper> CreateNoiseShaper(GeneratorSeed seed);
    static Scope<TerrainShaper> CreateSuperflatShaper(GeneratorSeed seed);

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

class SuperflatTerrainShaper : public TerrainShaper
{
public:
    SuperflatTerrainShaper(GeneratorSeed seed) : TerrainShaper(seed) {}
    virtual ~SuperflatTerrainShaper() = default;

    virtual float GetHeight(int x, int z) const override
    {
        return m_Height;
        // glm noise
        auto noise = glm::perlin(glm::vec2(x, z) * 0.1f + glm::vec2(m_Seed));
        return m_Height + noise * 5.0f;
    }

protected:
    int m_Height = 64;
};
