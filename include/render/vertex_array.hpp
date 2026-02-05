#pragma once
#include "buffer.hpp"
#include "vertex_buffer_layout.hpp"
#include <memory>

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

class VertexArrayNew
{
public:
    virtual ~VertexArrayNew() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const = 0;

    static std::shared_ptr<VertexArrayNew> Create();
};