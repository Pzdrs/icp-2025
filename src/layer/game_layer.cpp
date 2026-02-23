#include "pch.hpp"
#include "layer/game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "render/render_command.hpp"
#include "worldgen/world_generator.hpp"
#include "input.hpp"
#include "screenshot.hpp"
#include "audio/audio_engine.hpp"
#include "audio/audio.hpp"

static const std::string BLOCK_ATLAS = "assets/textures/blocks.png";
static const std::string SOUNDTRACK = "assets/audio/soundtrack/key.mp3";
static const std::string BLOCK_MANIFEST = "assets/misc/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_BlockAtlasHandle(Scuffcraft::Get().GetAssetManager().LoadAsset(BLOCK_ATLAS, AssetType::Texture2D)),
      m_SoundtrackHandle(Scuffcraft::Get().GetAssetManager().LoadAsset(SOUNDTRACK, AssetType::Audio)),
      m_World(CreateScope<OverworldGenerator>(
          GeneratorSeed(0),
          TerrainShaper::CreateNoiseShaper(GeneratorSeed(0)),
          SurfaceDecorator::CreateOverworldDecorator(GeneratorSeed(0))))
{
    BlockRegistry::Init(BLOCK_MANIFEST, m_BlockAtlasHandle, glm::vec2(16.0f, 16.0f));
    m_ShaderLibrary.Load("BlockShader", "assets/shaders/block.glsl");
    m_CameraController.SetPosition({7.0f, 200.0f, 7.0f});
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
    AudioEngine::PlayBackground(Scuffcraft::Get().GetAssetManager().GetAsset<Audio>(m_SoundtrackHandle));
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
    m_CameraController.OnUpdate(dt);

    m_World.OnUpdate(m_CameraController.GetCamera().GetPosition());

    m_World.ProcessCompletedJobs();

    RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    RenderCommand::Clear();

    Renderer::BeginScene(m_CameraController.GetCamera());

    m_World.Draw(m_ShaderLibrary.Get("BlockShader"));

    Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");
    ImGui::Text("WASD to move, mouse to look around");
    ImGui::Text("Press Left Alt + C to take a screenshot");
    ImGui::Text("Press Left Alt + Enter to toggle fullscreen");
    ImGui::Text("Press Escape to pause/unpause");
    ImGui::Text("Press Super (Command/Windows key) to zoom");
    ImGui::Text("Scroll to zoom in/out after holding Super");
    ImGui::End();

    ImGui::Begin("Audio controls");

    float masterVolume = AudioEngine::GetMasterVolume();
    float musicVolume = AudioEngine::GetMusicVolume();
    if (ImGui::SliderFloat("Master Volume", &masterVolume, 0.0f, 1.0f))
        AudioEngine::SetMasterVolume(masterVolume);
    if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 1.0f))
        AudioEngine::SetMusicVolume(musicVolume);

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