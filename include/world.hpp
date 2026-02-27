#pragma once

#include <glm/glm.hpp>
#include "worldgen/world_generator.hpp"
#include "block.hpp"
#include "render/material.hpp"
#include "chunk_manager.hpp"
#include <entt/entt.hpp>
#include "entity/entity_type.hpp"

class World
{
    static constexpr int RENDER_DISTANCE = 1;

public:
    World(Scope<WorldGenerator> generator);
    ~World() = default;

    void Draw(const Ref<Material> &blockMaterial, const Ref<Material> &entityMaterial);
    void OnUpdate(const float dt, const glm::vec3 &playerPos);
    void ProcessCompletedJobs();

    entt::entity SummonEntity(EntityType type, const glm::vec3 &position);

    ChunkManager &GetChunkManager() { return m_ChunkManager; }

private:
    void UnloadFarChunks(ChunkPosition playerChunk);

    void UpdateEntities(const float dt);

private:
    entt::registry m_Registry;

    Scope<WorldGenerator> m_Generator;
    ChunkManager m_ChunkManager;

    ChunkPosition m_LastPlayerChunk = {INT32_MIN, INT32_MIN};

    bool m_Infinite = false;
    bool m_ChunkUpdatesEnabled = true;

    AssetHandle m_ChestMeshHandle, m_SteveMeshHandle, m_CreeperMeshHandle;

    glm::vec3 m_StevePosition;
};