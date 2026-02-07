#include "pch.hpp"
#include "render/shader.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_shader.hpp"

Ref<Shader> Shader::Create(const std::string &vertexPath, const std::string &fragmentPath)
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
    }
    return nullptr;
}