#include "debug_layer.hpp"
#include <imgui.h>
#include "render/renderer.hpp"
#include "scuffcraft.hpp"

DebugLayer::DebugLayer(const Camera &camera)
    : Layer("DebugLayer"), m_Camera(camera)
{
}

DebugLayer::~DebugLayer()
{
}

void DebugLayer::OnAttach()
{
}

void DebugLayer::OnDetach()
{
}

void DebugLayer::OnUpdate(float dt)
{
    const float smoothing = 0.1f;
    m_FrameTime = m_FrameTime * (1.0f - smoothing) + dt * smoothing;
}

void DebugLayer::OnImGuiRender()
{
    const auto &caps = Renderer::GetCapabilities();
    const auto fps = 1.0f / m_FrameTime;

    ImGui::Begin("Debug Overlay", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Renderer: %s", caps.GraphicsAPI.c_str());
    ImGui::Text("Vendor: %s", caps.Vendor.c_str());
    ImGui::Text("Device: %s", caps.DeviceName.c_str());
    ImGui::Text("Driver: %s", caps.DriverVersion.c_str());
    ImGui::Text("GLSL: %s", caps.ShaderLanguage.c_str());
    ImGui::Separator();

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame Time: %.2f ms", m_FrameTime * 1000.0f);
    bool vsync = Scuffcraft::Get().GetWindow().IsVSync();
    if (ImGui::Checkbox("VSync", &vsync))
    {
        Scuffcraft::Get().GetWindow().SetVSync(vsync);
    }
    ImGui::Text("Draw Calls: %d", Renderer::GetStats().DrawCalls);
    ImGui::Text("Triangles: %d", Renderer::GetStats().TriangleCount);

    ImGui::Separator();

    ImGui::Text("Camera Pos: %.2f, %.2f, %.2f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
    if (auto perspectiveCamera = dynamic_cast<const PerspectiveCamera *>(&m_Camera))
    {
        ImGui::Text("Pitch/Yaw: %.2f / %.2f", perspectiveCamera->GetPitch(), perspectiveCamera->GetYaw());
    }

    ImGui::End();
}

void DebugLayer::OnEvent(Event &event)
{
}