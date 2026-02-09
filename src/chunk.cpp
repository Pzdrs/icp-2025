#include "pch.hpp"
#include "chunk.hpp"

#include <render/vertex.hpp>
#include <block.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "world.hpp"
#include <render/texture.hpp>
#include "block_registry.hpp"

Chunk::~Chunk()
{
    std::cout << "Destroying Chunk\n";
}

Chunk::Chunk(const World &world, const glm::vec2 &worldPos)
    : m_WorldPos(worldPos), m_World(world), va(VertexArray::Create())
{
    std::cout << "Creating Chunk at (" << worldPos.x << ", " << worldPos.y << ")\n";
}

glm::ivec2 Chunk::GetChunkCoords(float worldX, float worldZ)
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

bool Chunk::IsFaceExposed(int chunkX, int y, int chunkZ, Block::Face face, Block::ID type, const BlockRegistry &blockRegistry) const
{
    glm::ivec3 d = Block::GetFaceDirection(face);
    int nx = chunkX + d.x;
    int ny = y + d.y;
    int nz = chunkZ + d.z;

    // Outside chunk vertically = visible face
    if (ny < 0 || ny >= SIZE_Y)
        return true;

    // Outside chunk horizontally = check neighbor chunk
    if (nx < 0 || nx >= SIZE_XZ ||
        nz < 0 || nz >= SIZE_XZ)
    {
        // get neighbor chunk
        Chunk *neighborChunk = m_World.GetChunk(m_WorldPos.x + d.x, m_WorldPos.y + d.z);
        if (!neighborChunk)
        {
            // std::cout << "no neighbor chunk at (" << m_WorldPos.x + d.x << ", " << m_WorldPos.y + d.z << ")\n";
            return true;
        }
        return true;
    }

    Block::ID neighborType = blocks[nx][ny][nz].type;
    BlockDefinition nDef = blockRegistry.Get(neighborType);

    // water
    if (blockRegistry.Get(type).id == "water" && nDef.id == "water")
        return false;

    return nDef.isSolid == false;
}

// TODO: cull chunk border faces
void Chunk::GenerateMesh(const BlockRegistry &blockRegistry)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int indexOffset = 0;

    for (int x = 0; x < SIZE_XZ; x++)
        for (int y = 0; y < SIZE_Y; y++)
            for (int z = 0; z < SIZE_XZ; z++)
            {
                Block::ID type = blocks[x][y][z].type;

                if (type == blockRegistry.GetID("air"))
                    continue;

                glm::vec3 blockPos(x, y, z);
                float alpha = blockRegistry.Get(type).alpha;

                // 6 faces per cube
                for (int face = 0; face < 6; face++)
                {

                    Block::Face blockFace = static_cast<Block::Face>(face);

                    if (!IsFaceExposed(x, y, z, blockFace, type, blockRegistry))
                        continue;

                    Ref<SubTexture2D> faceTex = blockRegistry.GetFaceTexture(type, Block::FaceToTextureFace(blockFace));

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
                        indices.push_back(indexOffset + Block::GetFaceIndex(i));

                    indexOffset += 4;
                }
            }

    auto vb = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
    auto ib = IndexBuffer::Create(indices.data(), indices.size());

    vb->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });
    va->AddVertexBuffer(std::move(vb));
    va->SetIndexBuffer(std::move(ib));

    va->Unbind();
    std::cout << "Generated mesh with " << vertices.size() << " vertices and " << indices.size() << " indices.\n";
}

void Chunk::Draw(const Ref<Shader> &shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos.x * SIZE_XZ, 0.0f, m_WorldPos.y * SIZE_XZ));
    Renderer::Submit(shader, va, model);
}