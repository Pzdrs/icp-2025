#include "pch.hpp"
#include "opengl_renderer_api.hpp"
#include <glad/glad.h>

void OpenGLRendererAPI::Init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

RendererCapabilities &OpenGLRendererAPI::GetCapabilities()
{
    static RendererCapabilities caps;
    caps.GraphicsAPI = "OpenGL";
    caps.Vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    caps.DeviceName = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    caps.DriverVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    caps.ShaderLanguage = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    glGetIntegerv(GL_MAJOR_VERSION, &caps.APIMajor);
    glGetIntegerv(GL_MINOR_VERSION, &caps.APIMinor);

    return caps;
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount)
{
    vertexArray->Bind();
    uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}