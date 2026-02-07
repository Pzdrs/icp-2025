#pragma once

#include "render/vertex_array.hpp"

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(Scope<VertexBuffer> vertexBuffer) override;
    virtual void SetIndexBuffer(Scope<IndexBuffer> indexBuffer) override;

    virtual const std::vector<Scope<VertexBuffer>> &GetVertexBuffers() const override { return m_VertexBuffers; };
    virtual const Scope<IndexBuffer> &GetIndexBuffer() const override { return m_IndexBuffer; };

private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::vector<Scope<VertexBuffer>> m_VertexBuffers;
    Scope<IndexBuffer> m_IndexBuffer;
};