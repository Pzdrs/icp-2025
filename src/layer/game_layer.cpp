#include "pch.hpp"
#include "layer/game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "render/render_command.hpp"
#include "world_generator.hpp"
#include "input.hpp"
#include <screenshot.hpp>

static const std::string BLOCK_ATLAS = "assets/textures/blocks.png";
static const std::string BLOCK_MANIFEST = "assets/misc/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_BlockAtlas(Texture2D::Create(BLOCK_ATLAS)),
      m_BlockRegistry(m_BlockAtlas, {16, 16})
{
    m_BlockRegistry.LoadManifest(BLOCK_MANIFEST);
    m_ShaderLibrary.Load("BlockShader", "assets/shaders/block.glsl");
    auto gen = OverworldGenerator(0, m_BlockRegistry);
    m_World.Generate(gen);
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
    ImGui::Begin("Controls");
    ImGui::Text("WASD to move, mouse to look around");
    ImGui::Text("Press Left Alt + C to take a screenshot");
    ImGui::Text("Press Left Alt + Enter to toggle fullscreen");
    ImGui::Text("Press Escape to pause/unpause");
    ImGui::End();
}

void GameLayer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameLayer::OnKeyPressed));

    m_CameraController.OnEvent(event);
}

void GameLayer::SetPaused(bool paused)
{
    m_Paused = paused;
    Scuffcraft::Get().GetWindow().SetMouseLocked(!paused);
    m_CameraController.SetPaused(paused);
}

bool GameLayer::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == Key::Escape)
    {
        SetPaused(!m_Paused);
        return true;
    }

    if (e.getKeyCode() == Key::Enter && Input::IsKeyPressed(Key::LeftAlt))
    {
        m_Fullscreen = !m_Fullscreen;
        Scuffcraft::Get().GetWindow().SetFullscreen(m_Fullscreen);
        m_CameraController.ResetMouse();
        return true;
    }

    if (e.getKeyCode() == Key::C && Input::IsKeyPressed(Key::LeftAlt))
    {
        SetPaused(true);
        // threading candidate
        ScreenshotManager::Capture(Scuffcraft::Get().GetWindow());
        SetPaused(false);
        m_CameraController.ResetMouse();
        return true;
    }
    return false;
}