#include "pch.hpp"
#include "render/renderer.hpp"
#include "render/renderer3d.hpp"
#include "render/buffer.hpp"
#include "render/render_command.hpp"
#include "screenshot.hpp"

void Renderer::Init()
{
    RenderCommand::Init();
    Renderer3D::Init();

    s_Capabilities = RenderCommand::GetCapabilities();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Shutdown()
{
    Renderer3D::Shutdown();
}

void Renderer::BeginScene(const Camera &camera)
{
    s_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    s_Stats.Reset();
}

void Renderer::EndScene()
{
    // Code to end the current scene would go here
}

Screenshot Renderer::CaptureScreenshot(const unsigned int width, const unsigned int height)
{
    return {
        width,
        height,
        RenderCommand::CaptureScreenshot(width, height)};
}