#include "pch.hpp"
#include "render/renderer.hpp"
#include <render/buffer.hpp>
#include "render/render_command.hpp"

RendererCapabilities Renderer::s_Capabilities;
Statistics Renderer::s_Stats;
Scope<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

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

void Renderer::BeginScene(const Camera &camera)
{
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    s_Stats.Reset();
}

void Renderer::EndScene()
{
    // Code to end the current scene would go here
}

void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform)
{
    shader->Bind();
    shader->SetMat4("uViewProjection", s_SceneData->ViewProjectionMatrix);
    shader->SetMat4("uTransform", transform);

    RenderCommand::DrawIndexed(vertexArray);

    s_Stats.DrawCalls++;
    s_Stats.TriangleCount += vertexArray->GetIndexBuffer()->GetCount() / 3;
}