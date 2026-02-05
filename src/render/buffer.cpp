#include "render/buffer.hpp"
#include <render/renderer.hpp>
#include "../platform/opengl/opengl_buffer.hpp"

std::unique_ptr<VertexBuffer> VertexBuffer::Create(const void *data, uint32_t size)
{
    switch (RendererNew::CurrentAPI())
    {
    case RendererAPI::None:
        // assert
        return nullptr;
    case RendererAPI::OpenGL:
        return std::make_unique<OpenGLVertexBuffer>(data, size);
    }

    // assert
    return nullptr;
}

std::unique_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t *indices, uint32_t count)
{
    switch (RendererNew::CurrentAPI())
    {
    case RendererAPI::None:
        // assert
        return nullptr;
    case RendererAPI::OpenGL:
        return std::make_unique<OpenGLIndexBuffer>(indices, count);
    }

    // assert
    return nullptr;
}