#include "pch.hpp"
#include "game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "render/render_command.hpp"

static const std::string BLOCK_ATLAS = "assets/textures/blocks.png";
static const std::string BLOCK_MANIFEST = "assets/misc/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_Shader(Shader::Create("assets/shaders/texture.glsl")),
      m_BlockAtlas(Texture2D::Create(BLOCK_ATLAS))
{
    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);
    m_World.Generate(m_BlockRegistry);

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

    Renderer::BeginScene(m_CameraController.GetCamera());

    m_World.Draw(m_Shader, m_BlockRegistry);

    Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::OnKeyPressed));

    m_CameraController.OnEvent(event);
}

bool GameLayer::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == Key::Escape)
    {
        m_Paused = !m_Paused;
        Scuffcraft::Get().GetWindow().SetMouseLocked(!m_Paused);
        m_CameraController.SetPaused(m_Paused);
        return true;
    }
    return false;
}
