#pragma once
#include "render/vertex_array.hpp"
#include "render/index_buffer.hpp"
#include "render/renderer.hpp"
#include "block.hpp"

class Chunk
{
public:
    Chunk();
    ~Chunk();
    static const int SIZE_XZ = 16;
    static const int SIZE_Y = 256;
    static VertexBufferLayout layout;
    Block blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];
    void generateMesh(const BlockRegistry &blockRegistry);
    void draw(const Renderer &renderer, const Shader &shader);
    bool isFaceExposed(int x, int y, int z, int face, const BlockRegistry &blockRegistry) const;

private:
    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;
};