#include "render/buffer.hpp"
#include <render/renderer.hpp>
#include "../platform/opengl/opengl_buffer.hpp"

VertexBufferNew *VertexBufferNew::Create(const void *data, uint32_t size)
{
    switch (RendererNew::CurrentAPI())
    {
    case RendererAPI::None:
        // assert
        return nullptr;
    case RendererAPI::OpenGL:
        return new OpenGLVertexBuffer(data, size);
    }

    // assert
    return nullptr;
}

IndexBufferNew *IndexBufferNew::Create(const uint32_t *indices, uint32_t count)
{
    switch (RendererNew::CurrentAPI())
    {
    case RendererAPI::None:
        // assert
        return nullptr;
    case RendererAPI::OpenGL:
        return new OpenGLIndexBuffer(indices, count);
    }

    // assert
    return nullptr;
}