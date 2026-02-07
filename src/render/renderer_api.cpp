#include "pch.hpp"
#include "render/renderer_api.hpp"
#include "../platform/opengl/opengl_renderer_api.hpp"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
    switch (s_API)
    {
    case RendererAPI::API::None:
        // assert
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLRendererAPI>();
    }

    // assert
    return nullptr;
}