#include "pch.hpp"
#include "worldgen/terrain_shaper.hpp"
#include "chunk.hpp"
#include "glm/glm.hpp"

Scope<TerrainShaper> TerrainShaper::CreateNoiseShaper(GeneratorSeed seed)
{
    return CreateScope<NoiseTerrainShaper>(seed);
}

Scope<TerrainShaper> TerrainShaper::CreateSuperflatShaper(GeneratorSeed seed)
{
    return CreateScope<SuperflatTerrainShaper>(seed);
}

void NoiseTerrainShaper::InitNoise()
{
    m_ContinentalnessNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    m_ContinentalnessNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    m_ContinentalnessNoise.SetFractalOctaves(2);
    m_ContinentalnessNoise.SetFrequency(0.003f);
    m_ContinentalnessNoise.SetFractalGain(1.7f);
}

void NoiseTerrainShaper::InitSpline()
{
    std::vector<double> x, y;

    for (const auto &p : CONTINENTALNESS_SPLINE)
    {
        x.push_back(p.first);  // x = noise
        y.push_back(p.second); // y = absolute height
    }

    m_ContinentalnessSpline = tk::spline(x, y, tk::spline::linear);
}

float NoiseTerrainShaper::GetHeight(int x, int z) const
{
    float noise = m_ContinentalnessNoise.GetNoise((float)x, (float)z);
    int height = m_ContinentalnessSpline(noise);
    height = glm::clamp(height, 1, Chunk::SIZE_Y - 1);
    return height;
}