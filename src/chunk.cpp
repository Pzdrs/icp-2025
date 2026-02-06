#include "chunk.hpp"
#include <iostream>
#include <render/vertex.hpp>
#include <atlas.hpp>
#include <block.hpp>
#include <glm/gtc/matrix_transform.hpp>

Chunk::~Chunk()
{
    std::cout << "Destroying Chunk\n";
}

Chunk::Chunk(const glm::vec2 &worldPos) : worldPos(worldPos), va(VertexArray::Create())
{
    std::cout << "Creating Chunk at (" << worldPos.x << ", " << worldPos.y << ")\n";
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

bool Chunk::isFaceExposed(int x, int y, int z, int face, const BlockRegistry &blockRegistry) const
{
    glm::ivec3 d = FACE_DIRS[face];

    int nx = x + d.x;
    int ny = y + d.y;
    int nz = z + d.z;

    // Outside chunk = visible face
    if (nx < 0 || nx >= SIZE_XZ ||
        ny < 0 || ny >= SIZE_Y ||
        nz < 0 || nz >= SIZE_XZ)
        return true;

    return blocks[nx][ny][nz].type == blockRegistry.getID("air");
}

void Chunk::generateMesh(const BlockRegistry &blockRegistry)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int indexOffset = 0;

    for (int x = 0; x < SIZE_XZ; x++)
        for (int y = 0; y < SIZE_Y; y++)
            for (int z = 0; z < SIZE_XZ; z++)
            {
                if (blocks[x][y][z].type == blockRegistry.getID("air"))
                    continue;

                glm::vec3 blockPos(x, y, z);

                // 6 faces per cube
                for (int face = 0; face < 6; face++)
                {
                    if (!isFaceExposed(x, y, z, face, blockRegistry))
                        continue;

                    // Add 4 vertices per face
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = CUBE_VERTS[face][v] + blockPos;
                        vert.color = {0.0f, 0.0f, 0.0f};

                        int uvIndex = FACE_UV_MAP[face][v];
                        vert.texCoord = getBlockUV(blocks[x][y][z].type, face, uvIndex, blockRegistry);

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
        {ShaderDataType::Float3, "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });
    va->AddVertexBuffer(std::move(vb));
    va->SetIndexBuffer(std::move(ib));

    va->Unbind();
    std::cout << "Generated mesh with " << vertices.size() << " vertices and " << indices.size() << " indices.\n";
}

void Chunk::draw(const std::shared_ptr<Shader> &shader)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(worldPos.x * SIZE_XZ, 0.0f, worldPos.y * SIZE_XZ));
    Renderer::Submit(shader, va, model);    
}
