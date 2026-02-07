#include "pch.hpp"
#include "render/buffer.hpp"
#include <render/renderer.hpp>
#include "../platform/opengl/opengl_buffer.hpp"

Scope<VertexBuffer> VertexBuffer::Create(const void *data, uint32_t size)
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        // assert
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLVertexBuffer>(data, size);
    }

    // assert
    return nullptr;
}

Scope<IndexBuffer> IndexBuffer::Create(const uint32_t *indices, uint32_t count)
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        // assert
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLIndexBuffer>(indices, count);
    }

    // assert
    return nullptr;
}