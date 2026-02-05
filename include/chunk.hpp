#pragma once
#include "render/vertex_array.hpp"
#include "render/renderer.hpp"
#include "block.hpp"
#include "render/buffer.hpp"

class Chunk
{
public:
    Chunk();
    ~Chunk();
    static const int SIZE_XZ = 16;
    static const int SIZE_Y = 256;
    Block blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];
    void generateMesh(const BlockRegistry &blockRegistry);
    void draw(const Shader &shader);
    bool isFaceExposed(int x, int y, int z, int face, const BlockRegistry &blockRegistry) const;

private:
    std::shared_ptr<VertexArray> va;
};