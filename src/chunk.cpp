#include "pch.hpp"
#include "chunk.hpp"

#include <render/vertex.hpp>
#include <block.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "world.hpp"
#include <render/texture.hpp>
#include "block_registry.hpp"
#include "render/renderer3d.hpp"
#include <thread>

Chunk::~Chunk()
{
    // std::cout << "Destroying Chunk at (" << m_Position.x << ", " << m_Position.y << ")\n";
}

Chunk::Chunk(const ChunkManager &chunkManager, ChunkPosition position)
    : m_Position(position), m_ChunkManager(chunkManager)
{
    // std::cout << "Creating Chunk at (" << position.x << ", " << position.y << ")\n";
}

ChunkPosition Chunk::GetChunkCoords(float worldX, float worldZ)
{
    int chunkX = static_cast<int>(std::floor(worldX / SIZE_XZ));
    int chunkZ = static_cast<int>(std::floor(worldZ / SIZE_XZ));
    return {chunkX, chunkZ};
}

glm::vec2 getFaceUV(const Ref<SubTexture2D> &subTexture, int vertexIndex)
{
    const glm::vec2 &c = FACE_UVS[vertexIndex];
    float u = c.x ? subTexture->GetTexCoords()[2].x : subTexture->GetTexCoords()[0].x;
    float v = c.y ? subTexture->GetTexCoords()[2].y : subTexture->GetTexCoords()[0].y;

    return {u, v};
}

bool Chunk::IsFaceExposed(int chunkX, int y, int chunkZ, Block::Face face, Block::ID type) const
{
    glm::ivec3 d = Block::GetFaceDirection(face);
    int nx = chunkX + d.x;
    int ny = y + d.y;
    int nz = chunkZ + d.z;

    if (ny < 0)
        return false; // dont render bottom face if at bottom of world
    if (ny >= SIZE_Y)
        return true;

    // Outside chunk horizontally = check neighbor chunk
    if (nx < 0 || nx >= SIZE_XZ || nz < 0 || nz >= SIZE_XZ)
    {
        // Determine which neighbor chunk to check
        int neighborChunkX = m_Position.x + (nx < 0 ? -1 : (nx >= SIZE_XZ ? 1 : 0));
        int neighborChunkZ = m_Position.y + (nz < 0 ? -1 : (nz >= SIZE_XZ ? 1 : 0));

        Chunk *neighborChunk = m_ChunkManager.GetChunk(neighborChunkX, neighborChunkZ);
        if (!neighborChunk)
        {
            return false;
        } // treat missing neighbor as solid (prevents holes)

        // Wrap local coordinates into neighbor chunk
        int neighborLocalX = (nx + SIZE_XZ) % SIZE_XZ;
        int neighborLocalZ = (nz + SIZE_XZ) % SIZE_XZ;

        Block::ID neighborType = neighborChunk->GetBlock(neighborLocalX, ny, neighborLocalZ).type;
        const BlockDefinition &nDef = BlockRegistry::Get().Get(neighborType);

        // special case: water
        if (BlockRegistry::Get().Get(type).id == "water" && nDef.id == "water")
            return false;

        return !nDef.isSolid;
    }

    // Inside current chunk
    Block::ID neighborType = blocks[nx][ny][nz].type;
    const BlockDefinition &nDef = BlockRegistry::Get().Get(neighborType);

    // special case: water
    if (BlockRegistry::Get().Get(type).id == "water" && nDef.id == "water")
        return false;

    return !nDef.isSolid;
}

Chunk::Mesh Chunk::BuildMesh()
{
    LOG("Chunk::BuildMesh");
    Mesh mesh;

    unsigned int solidIndexOffset = 0;

    for (int x = 0; x < SIZE_XZ; x++)
        for (int y = 0; y < SIZE_Y; y++)
            for (int z = 0; z < SIZE_XZ; z++)
            {
                Block::ID type = blocks[x][y][z].type;

                if (type == BlockRegistry::Get().GetID("air"))
                    continue;

                glm::vec3 blockPos(x, y, z);
                float alpha = BlockRegistry::Get().Get(type).alpha;

                // Determine whether block is solid or transparent
                bool isTransparent = alpha < 1.0f || BlockRegistry::Get().Get(type).id == "water";

                if (isTransparent)
                {
                    continue;
                } // skip transparent blocks for now (TODO: separate solid vs transparent meshes)

                // 6 faces per cube
                for (int face = 0; face < 6; face++)
                {
                    Block::Face blockFace = static_cast<Block::Face>(face);

                    if (!IsFaceExposed(x, y, z, blockFace, type))
                        continue;

                    Ref<SubTexture2D> faceTex = BlockRegistry::Get().GetFaceTexture(type, Block::FaceToTextureFace(blockFace));

                    // Reference to correct mesh (solid vs transparent)
                    auto &vertices = mesh.solidVertices;
                    auto &indices = mesh.solidIndices;

                    // Add 4 vertices per face
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = Block::GetVertexPosition(blockFace, v) + blockPos;
                        vert.color = {0.0f, 0.0f, 0.0f, alpha};

                        int uvIndex = FACE_UV_MAP[face][v];
                        vert.texCoord = getFaceUV(faceTex, uvIndex);

                        vertices.push_back(vert);
                    }

                    // Add 6 indices per face
                    for (int i = 0; i < 6; i++)
                        indices.push_back(solidIndexOffset + Block::GetFaceIndex(i));
                    solidIndexOffset += 4;
                }
            }

    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate expensive mesh generation

    return mesh;
}

void Chunk::UploadMesh(const Mesh &mesh)
{
    LOG("Chunk::UploadMesh (with mesh)");
    m_SolidVA = VertexArray::Create();

    auto vb = VertexBuffer::Create(mesh.solidVertices.data(), mesh.solidVertices.size() * sizeof(Vertex));
    auto ib = IndexBuffer::Create(mesh.solidIndices.data(), mesh.solidIndices.size());

    vb->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });
    m_SolidVA->AddVertexBuffer(std::move(vb));
    m_SolidVA->SetIndexBuffer(std::move(ib));

    m_MeshState = MeshState::READY;
}

void Chunk::Draw(const Ref<Shader> &shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position.x * SIZE_XZ, 0.0f, m_Position.y * SIZE_XZ));
    Renderer3D::DrawMesh(shader, m_SolidVA, model);
}