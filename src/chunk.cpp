#include "chunk.hpp"
#include <iostream>
#include <render/vertex.hpp>
#include <atlas.hpp>

static const glm::vec3 cubeVerts[6][4] = {
    // Back face (z = 0)
    {{0, 0, 0},   // back top left
     {1, 0, 0},   // back top right
     {1, -1, 0},  // back bottom right
     {0, -1, 0}}, // back bottom left

    // Front face (z = 1)
    {{0, 0, 1},
     {1, 0, 1},
     {1, -1, 1},
     {0, -1, 1}},

    // Left face (x = 0)
    {{0, 0, 1},
     {0, 0, 0},
     {0, -1, 0},
     {0, -1, 1}},

    // Right face (x = 1)
    {{1, 0, 0},
     {1, 0, 1},
     {1, -1, 1},
     {1, -1, 0}},

    // Top face (y = 0)
    {{0, 0, 0},
     {1, 0, 0},
     {1, 0, 1},
     {0, 0, 1}},

    // Bottom face (y = -1)
    {{0, -1, 1},
     {1, -1, 1},
     {1, -1, 0},
     {0, -1, 0}},
};

static const unsigned int quadIndices[6] = {
    0, 1, 2,
    2, 3, 0};

static const glm::vec2 quadUVs[4] = {
    {0, 1}, // 0 = top-left
    {1, 1}, // 1 = top-right
    {1, 0}, // 2 = bottom-right
    {0, 0}, // 3 = bottom-left
};

VertexBufferLayout Chunk::layout;

Chunk::~Chunk()
{
    std::cout << "Destroying Chunk\n";
}

Chunk::Chunk()
{
    std::cout << "Creating Chunk\n";
}

glm::vec2 getBlockUV(BlockType type, int vertexIndex)
{
    UVRect uv = getSpriteUV(17, 7, 16, 1024, 1024);

    const glm::vec2 &c = quadUVs[vertexIndex];
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
                    // Add 4 vertices
                    for (int v = 0; v < 4; v++)
                    {
                        Vertex vert;
                        vert.position = cubeVerts[face][v] + blockPos;
                        vert.color = {0.0f, 0.0f, 0.0f};
                        vert.texCoord = getBlockUV(blocks[x][y][z].type, v);

                        vertices.push_back(vert);
                    }

                    // Add indices
                    for (int i = 0; i < 6; i++)
                        indices.push_back(indexOffset + quadIndices[i]);

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
