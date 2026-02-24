#pragma once

#include <glm/glm.hpp>
#include "worldgen/world_generator.hpp"
#include "block.hpp"
#include "render/shader.hpp"
#include "chunk_manager.hpp"

class World
{
    static constexpr int RENDER_DISTANCE = 16;

public:
    World(Scope<WorldGenerator> generator) : m_Generator(std::move(generator)) {}
    ~World() = default;

    void Draw(const Ref<Shader> &shader);
    void OnUpdate(const glm::vec3 &playerPos);
    void ProcessCompletedJobs();

    ChunkManager &GetChunkManager() { return m_ChunkManager; }

private:
    void UnloadFarChunks(ChunkPosition playerChunk);

private:
    Scope<WorldGenerator> m_Generator;
    ChunkManager m_ChunkManager;

    ChunkPosition m_LastPlayerChunk = {INT32_MIN, INT32_MIN};
};