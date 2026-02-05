#include "render/renderer.hpp"
#include <render/buffer.hpp>
#include "render/render_command.hpp"

void Renderer::Init()
{
    RenderCommand::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Shutdown()
{
    // Cleanup code for Renderer would go here
}

void Renderer::BeginScene()
{
    // Code to begin a new scene would go here
}

void Renderer::EndScene()
{
    // Code to end the current scene would go here
}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
{
    RenderCommand::DrawIndexed(vertexArray);
}