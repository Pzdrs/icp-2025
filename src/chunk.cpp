#include "chunk.hpp"
#include <iostream>
#include <render/vertex.hpp>
#include <atlas.hpp>
#include <block.hpp>

VertexBufferLayout Chunk::layout;

Chunk::~Chunk()
{
    std::cout << "Destroying Chunk\n";
}

Chunk::Chunk()
{
    std::cout << "Creating Chunk\n";
}

glm::vec2 getBlockUV(BlockType type, int face, int vertexIndex)
{
    // actual UVs on the atlas
    UVRect uv;
    if (face == 0 || face == 1 || face == 2 || face == 3)
        uv = getSpriteUV(20, 6, 16, 1024, 1024);
    else if (face == 4)
        uv = getSpriteUV(21, 5, 16, 1024, 1024);
    else // face == 5
        uv = getSpriteUV(17, 10, 16, 1024, 1024);

    const glm::vec2 &c = FACE_UVS[vertexIndex];
    float u = c.x ? uv.u1 : uv.u0;
    float v = c.y ? uv.v1 : uv.v0;

    return {u, v};
}

void Chunk::generateMesh()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int indexOffset = 0;

    for (int x = 0; x < SIZE_XZ; x++)
        for (int y = 0; y < SIZE_Y; y++)
            for (int z = 0; z < SIZE_XZ; z++)
            {
                if (blocks[x][y][z].type == BlockType::AIR)
                    continue;

                glm::vec3 blockPos(x, y, z);

                // 6 faces per cube
                for (int face = 0; face < 6; face++)
                {
                    // Add 4 vertices per face
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = CUBE_VERTS[face][v] + blockPos;
                        vert.color = {0.0f, 0.0f, 0.0f};

                        int uvIndex = FACE_UV_MAP[face][v];
                        vert.texCoord = getBlockUV(blocks[x][y][z].type, face, uvIndex);

                        vertices.push_back(vert);
                    }

                    // Add 6 indices per face
                    for (int i = 0; i < 6; i++)
                        indices.push_back(indexOffset + FACE_INDICES[i]);

                    indexOffset += 4;
                }
            }

    vb = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    ib = IndexBuffer(indices.data(), indices.size());

    va.addBuffer(vb, layout);

    va.unbind();
    vb.unbind();
    ib.unbind();
    std::cout << "Generated mesh with " << vertices.size() << " vertices and " << indices.size() << " indices.\n";
}

void Chunk::draw(const Renderer &renderer, const Shader &shader)
{
    renderer.draw(va, ib, shader);
}
