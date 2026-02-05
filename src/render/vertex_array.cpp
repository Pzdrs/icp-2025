#include "render/vertex_array.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_vertex_array.hpp"

std::unique_ptr<VertexArray> VertexArray::Create()
{
    switch (RendererNew::CurrentAPI())
    {
    case RendererAPI::None:
        return nullptr;
    case RendererAPI::OpenGL:
        return std::make_unique<OpenGLVertexArray>();
    }

    // assert
    return nullptr;
}