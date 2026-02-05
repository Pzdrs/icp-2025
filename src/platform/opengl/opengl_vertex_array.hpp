#pragma once

#include "render/vertex_array.hpp"

class OpenGLVertexArray : public VertexArrayNew
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) override;
    virtual void SetIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) override;

    virtual const std::vector<std::unique_ptr<VertexBuffer>> &GetVertexBuffers() const override { return m_VertexBuffers; };
    virtual const std::unique_ptr<IndexBuffer> &GetIndexBuffer() const override { return m_IndexBuffer; };
private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex = 0;
    std::vector<std::unique_ptr<VertexBuffer>> m_VertexBuffers;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
};