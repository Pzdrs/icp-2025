#include "render/renderer.hpp"
#include <render/buffer.hpp>
#include "render/render_command.hpp"

RendererCapabilities Renderer::s_Capabilities;
Statistics Renderer::s_Stats;

void Renderer::Init()
{
    RenderCommand::Init();

    s_Capabilities = RenderCommand::GetCapabilities();
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
    s_Stats.Reset();
}

void Renderer::EndScene()
{
    // Code to end the current scene would go here
}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
{
    RenderCommand::DrawIndexed(vertexArray);
    s_Stats.DrawCalls++;
    s_Stats.TriangleCount += vertexArray->GetIndexBuffer()->GetCount() / 3;
}