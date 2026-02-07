#include "render/vertex_array.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_vertex_array.hpp"

Ref<VertexArray> VertexArray::Create()
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        return nullptr;
    case RendererAPI::API::OpenGL:
        return std::make_shared<OpenGLVertexArray>();
    }

    // assert
    return nullptr;
}