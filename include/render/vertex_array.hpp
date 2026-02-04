#pragma once
#include "buffer.hpp"
#include "vertex_buffer_layout.hpp"

// VAO knows how to interpret vertex buffer data
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const;
    void unbind() const;

private:
    unsigned int rendererID;
};