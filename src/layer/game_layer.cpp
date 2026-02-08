#include "pch.hpp"
#include "layer/game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "render/render_command.hpp"
#include "world_generator.hpp"

static const std::string BLOCK_ATLAS = "assets/textures/blocks.png";
static const std::string BLOCK_MANIFEST = "assets/misc/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_BlockAtlas(Texture2D::Create(BLOCK_ATLAS))
{
    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);
    m_ShaderLibrary.Load("BlockShader", "assets/shaders/block.glsl");
    auto gen = OverworldGenerator(0, m_BlockRegistry);
    m_World.Generate(gen);

    m_CameraController.SetPosition(glm::vec3(160.0f, 400.0f, 200.0f));
    m_CameraController.SetPitchYaw(-90.0f, 0.0f);
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

    m_World.Draw(m_ShaderLibrary.Get("BlockShader"), m_BlockRegistry);

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
