#pragma once
#include "render/vertex_array.hpp"
#include "render/index_buffer.hpp"
#include "render/renderer.hpp"

enum class BlockType : uint16_t
{
    AIR = 0,
    DIRT = 1,
    GRASS = 2,
    STONE = 3,
};

struct Block
{
    BlockType type;
};

class Chunk
{
public:
    Chunk();
    ~Chunk();
    static const int SIZE_XZ = 16;
    static const int SIZE_Y = 1;
    static VertexBufferLayout layout;
    Block blocks[SIZE_XZ][SIZE_Y][SIZE_XZ];
    void generateMesh();
    void draw(const Renderer &renderer, const Shader &shader);

private:
    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;
};