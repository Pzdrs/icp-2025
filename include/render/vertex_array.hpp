#pragma once
#include "buffer.hpp"

class VertexArray
{
public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(Scope<VertexBuffer> vertexBuffer) = 0;
    virtual void SetIndexBuffer(Scope<IndexBuffer> indexBuffer) = 0;

    virtual const std::vector<Scope<VertexBuffer>> &GetVertexBuffers() const = 0;
    virtual const Scope<IndexBuffer> &GetIndexBuffer() const = 0;

    static Scope<VertexArray> Create();
};