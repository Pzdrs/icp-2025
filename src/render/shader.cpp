#include "render/shader.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_shader.hpp"

std::shared_ptr<Shader> Shader::Create(const std::string &vertexPath, const std::string &fragmentPath)
{
    switch (Renderer::CurrentAPI())
    {
        case RendererAPI::API::None:
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
    }
    return nullptr;
}