#include "pch.hpp"
#include "render/texture.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_texture.hpp"

Ref<Texture2D> Texture2D::Create(const std::string &path)
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(path);
    }

    // assert
    return nullptr;
}