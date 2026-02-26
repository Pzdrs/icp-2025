#pragma once

#include <glm/glm.hpp>
#include "worldgen/world_generator.hpp"
#include "block.hpp"
#include "render/shader.hpp"
#include "chunk_manager.hpp"

class World
{
    static constexpr int RENDER_DISTANCE = 1;

public:
    World(Scope<WorldGenerator> generator);
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

    bool m_Infinite = false;
    bool m_ChunkUpdatesEnabled = true;

    AssetHandle m_ChestMeshHandle;
    AssetHandle m_SteveMeshHandle;
};