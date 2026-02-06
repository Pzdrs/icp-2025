#include "game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "atlas.hpp"
#include "render/render_command.hpp"

static const std::string BLOCK_ATLAS = "resources/blocks.png";
static const std::string BLOCK_MANIFEST = "resources/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().getWidth() / (float)Scuffcraft::Get().GetWindow().getHeight()),
      m_Shader("shaders/shader.vert", "shaders/shader.frag")
{
    initAtlas(BLOCK_ATLAS);
    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);
    m_World.generate(m_BlockRegistry);

    m_CameraController.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_CameraController.SetPitchYaw(0.0f, -90.0f);
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
    m_CameraController.OnUpdate(dt);

    RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    RenderCommand::Clear();

    Renderer::BeginScene();

    m_Shader.setMat4("uViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
    m_Shader.setMat4("uTransform", glm::mat4(1.0f));

    m_World.draw(m_Shader);

    Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
    ImGui::ShowDemoWindow();
}

void GameLayer::OnEvent(Event &event)
{
    m_CameraController.OnEvent(event);
}
