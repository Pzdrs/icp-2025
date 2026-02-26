#include "pch.hpp"
#include "world.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "scuffcraft.hpp"
#include "render/renderer3d.hpp"
#include "mesh.hpp"
#include <glm/glm.hpp>

World::World(Scope<WorldGenerator> generator)
    : m_Generator(std::move(generator))
{
    m_ChestMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/chest.obj", AssetType::Mesh);
    m_SteveMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/steve.obj", AssetType::Mesh);
}

void World::Draw(const Ref<Material> &blockMaterial, const Ref<Material> &entityMaterial)
{
    Renderer3D::DrawMesh(Scuffcraft::Get().GetAssetManager().GetAsset<StaticMesh>(m_SteveMeshHandle)->GetVertexArray(), entityMaterial, glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 65.0f, 2.0f)));

    Renderer3D::DrawMesh(Scuffcraft::Get().GetAssetManager().GetAsset<StaticMesh>(m_ChestMeshHandle)->GetVertexArray(), blockMaterial, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 65.0f, 0.0f)));

    for (auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        if (chunk->GetMeshState() == MeshState::READY)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetPosition().x * Chunk::SIZE_XZ, 0.0f, chunk->GetPosition().y * Chunk::SIZE_XZ));
            Renderer3D::DrawMesh(chunk->GetSolidVA(), blockMaterial, model);
        }
    }
}

void World::OnUpdate(const float dt, const glm::vec3 &playerPos)
{
    if (!m_ChunkUpdatesEnabled)
        return;

    if (m_Infinite)
    {
        ChunkPosition playerChunk = Chunk::GetChunkCoords(playerPos.x, playerPos.z);
        if (playerChunk == m_LastPlayerChunk)
            return;

        m_LastPlayerChunk = playerChunk;

        std::vector<ChunkPosition> requiredChunks;

        for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; dx++)
        {
            for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; dz++)
            {
                ChunkPosition pos = playerChunk + ChunkPosition(dx, dz);

                if (!m_ChunkManager.HasChunk(pos))
                    requiredChunks.push_back(pos);
            }
        }

        std::sort(requiredChunks.begin(), requiredChunks.end(),
                  [playerChunk](const ChunkPosition &a, const ChunkPosition &b)
                  {
                      auto da = glm::length2(glm::vec2(a - playerChunk));
                      auto db = glm::length2(glm::vec2(b - playerChunk));
                      return da < db;
                  });

        for (const auto &pos : requiredChunks)
            m_ChunkManager.EnsureChunkExists(pos, *m_Generator);

        UnloadFarChunks(playerChunk);
    }
    else
    {
        ChunkPosition playerChunk = {0, 0};
        int worldDistance = 1;

        std::vector<ChunkPosition> requiredChunks;
        for (int dx = -worldDistance; dx <= worldDistance; dx++)
        {
            for (int dz = -worldDistance; dz <= worldDistance; dz++)
            {
                ChunkPosition pos = playerChunk + ChunkPosition(dx, dz);

                if (!m_ChunkManager.HasChunk(pos))
                    requiredChunks.push_back(pos);
            }
        }

        std::sort(requiredChunks.begin(), requiredChunks.end(),
                  [playerChunk](const ChunkPosition &a, const ChunkPosition &b)
                  {
                      auto da = glm::length2(glm::vec2(a - playerChunk));
                      auto db = glm::length2(glm::vec2(b - playerChunk));
                      return da < db;
                  });

        for (const auto &pos : requiredChunks)
            m_ChunkManager.EnsureChunkExists(pos, *m_Generator);

        m_ChunkUpdatesEnabled = false;
    }
}

void World::ProcessCompletedJobs()
{
    m_ChunkManager.ProcessCompletedJobs();
}

void World::UnloadFarChunks(ChunkPosition playerChunk)
{
    std::vector<ChunkPosition> toUnload;
    for (const auto &[pos, chunk] : m_ChunkManager.GetChunks())
    {
        int dx = pos.x - playerChunk.x;
        int dz = pos.y - playerChunk.y;

        if (std::abs(dx) > RENDER_DISTANCE || std::abs(dz) > RENDER_DISTANCE)
            toUnload.push_back(pos);
    }

    for (const auto &pos : toUnload)
        m_ChunkManager.UnloadChunk(pos);
}
