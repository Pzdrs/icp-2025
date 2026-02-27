#pragma once

#include <glm/glm.hpp>
#include "worldgen/world_generator.hpp"
#include "block.hpp"
#include "render/material.hpp"
#include "world/chunk_manager.hpp"
#include <entt/entt.hpp>

class Entity;

class World
{
    static constexpr int RENDER_DISTANCE = 1;

public:
    World(Scope<WorldGenerator> generator);
    ~World() = default;

    void Draw(const Ref<Material> &blockMaterial);
    void OnUpdate(const float dt, const glm::vec3 &playerPos);
    void ProcessCompletedJobs();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

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

    friend class Entity;
};