#pragma once

#include "block.hpp"

using GeneratorSeed = uint64_t;

class WorldGenerator
{
public:
    WorldGenerator(GeneratorSeed seed, BlockRegistry blockRegistry) : m_Seed(seed), m_BlockRegistry(blockRegistry) {}
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
    OverworldGenerator(GeneratorSeed seed, BlockRegistry blockRegistry) : WorldGenerator(seed, blockRegistry) {}
    virtual ~OverworldGenerator();

    virtual Block GetBlock(int x, int y, int z) const override;

private:
    int m_BaseHeight = 8;
    int m_Amplitude = 6;
};