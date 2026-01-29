#include "chunk.hpp"
#include <iostream>
#include <render/vertex.hpp>

static const glm::vec3 cubeVerts[6][4] = {
    // Front
    {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}},
    // Back
    {{1, 0, 0}, {0, 0, 0}, {0, 1, 0}, {1, 1, 0}},
    // Left
    {{0, 0, 0}, {0, 0, 1}, {0, 1, 1}, {0, 1, 0}},
    // Right
    {{1, 0, 1}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}},
    // Top
    {{0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 0}},
    // Bottom
    {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}},
};

static const unsigned int quadIndices[6] = {
    0, 1, 2,
    2, 3, 0};

static const glm::vec2 quadUVs[4] = {
    {0, 0}, {1, 0}, {1, 1}, {0, 1}};

VertexBufferLayout Chunk::layout;

Chunk::~Chunk()
{
    std::cout << "Destroying Chunk\n";
}

Chunk::Chunk() : vb(nullptr, 0), ib(nullptr, 0)
{
    std::cout << "Creating Chunk\n";
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
                        vert.position = blockPos + cubeVerts[face][v];
                        vert.color = {1.0f, 1.0f, 1.0f};
                        vert.texCoord = quadUVs[v];

                        vertices.push_back(vert);
                    }

                    // Add indices
                    for (int i = 0; i < 6; i++)
                        indices.push_back(indexOffset + quadIndices[i]);

                    indexOffset += 4;
                }
            }

    va.bind();

    vb = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    ib = IndexBuffer(indices.data(), indices.size());

    va.addBuffer(vb, layout);

    va.unbind();
}

void Chunk::draw(const Renderer &renderer, const Shader &shader)
{
    renderer.draw(va, ib, shader);
}
