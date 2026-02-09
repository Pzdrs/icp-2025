#include "pch.hpp"
#include "chunk.hpp"

#include <render/vertex.hpp>
#include <block.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "world.hpp"
#include <render/texture.hpp>

struct UVRect
{
    float u0, v0; // bottom-left
    float u1, v1; // top-right
};

UVRect getSpriteUV(int col, int row, int spriteSize, int atlasWidth, int atlasHeight);

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

glm::vec2 getBlockUV(BlockID type, int faceIndex, int vertexIndex, const BlockRegistry &blockRegistry)
{
    Face face = static_cast<Face>(faceIndex);
    BlockTexture tex = getBlockTexture(type, blockRegistry);
    // actual UVs on the atlas
    UVRect uv;
    if (isSideFace(face))
        uv = getSpriteUV(tex.side.u, tex.side.v, 16, 1024, 1024);
    else if (face == Face::FACE_UP)
        uv = getSpriteUV(tex.top.u, tex.top.v, 16, 1024, 1024);
    else // face == 5
        uv = getSpriteUV(tex.bottom.u, tex.bottom.v, 16, 1024, 1024);

    const glm::vec2 &c = FACE_UVS[vertexIndex];
    float u = c.x ? uv.u1 : uv.u0;
    float v = c.y ? uv.v1 : uv.v0;

    return {u, v};
}

bool Chunk::IsFaceExposed(int chunkX, int y, int chunkZ, int face, BlockID type, const BlockRegistry &blockRegistry) const
{
    glm::ivec3 d = FACE_DIRS[face];
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
        if (!neighborChunk) {
            std::cout << "no neighbor chunk at (" << m_WorldPos.x + d.x << ", " << m_WorldPos.y + d.z << ")\n";
            return true;
        }
        return true;
    }

    BlockID neighborType = blocks[nx][ny][nz].type;
    BlockDefinition nDef = blockRegistry.get(neighborType);

    // water
    if (blockRegistry.get(type).id == "water" && nDef.id == "water")
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
                BlockID type = blocks[x][y][z].type;

                if (type == blockRegistry.getID("air"))
                    continue;

                glm::vec3 blockPos(x, y, z);

                // 6 faces per cube
                for (int face = 0; face < 6; face++)
                {
                    if (!IsFaceExposed(x, y, z, face, type, blockRegistry))
                        continue;

                    // Add 4 vertices per face
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = CUBE_VERTS[face][v] + blockPos;
                        vert.color = {0.0f, 0.0f, 0.0f, blockRegistry.get(type).alpha};

                        int uvIndex = FACE_UV_MAP[face][v];
                        vert.texCoord = getBlockUV(type, face, uvIndex, blockRegistry);

                        vertices.push_back(vert);
                    }

                    // Add 6 indices per face
                    for (int i = 0; i < 6; i++)
                        indices.push_back(indexOffset + FACE_INDICES[i]);

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

// Get UV coordinates for a sprite at (col, row)
UVRect getSpriteUV(int col, int row, int spriteSize, int atlasWidth, int atlasHeight)
{
    float uSize = (float)spriteSize / atlasWidth;
    float vSize = (float)spriteSize / atlasHeight;

    int rows = atlasHeight / spriteSize;
    int flippedRow = rows - 1 - row;

    // OpenGL UV origin is bottom-left
    float u0 = col * uSize;
    float v0 = flippedRow * vSize;
    float u1 = u0 + uSize;
    float v1 = v0 + vSize;

    return {u0, v0, u1, v1};
}