#include "pch.hpp"
#include "layer/game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "world/chunk.hpp"
#include "render/render_command.hpp"
#include "worldgen/world_generator.hpp"
#include "input.hpp"
#include "screenshot.hpp"
#include "audio/audio_engine.hpp"
#include "audio/audio.hpp"
#include "music_manager.hpp"
#include "time.hpp"
#include "world/entity.hpp"
#include "world/components.hpp"

static const std::string BLOCK_ATLAS = "assets/textures/blocks.png";
static const std::string MUSIC_DIR = "assets/audio/soundtrack";
static const std::string BLOCK_MANIFEST = "assets/misc/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_BlockAtlasHandle(Scuffcraft::Get().GetAssetManager().LoadAsset(BLOCK_ATLAS, AssetType::Texture2D)),
      m_AwMan(Scuffcraft::Get().GetAssetManager().LoadAsset("assets/audio/entity/creeper.mp3", AssetType::Audio)),
      m_World(CreateScope<OverworldGenerator>(
          GeneratorSeed(0),
          TerrainShaper::CreateSuperflatShaper(GeneratorSeed(0)),
          SurfaceDecorator::CreateOverworldDecorator(GeneratorSeed(0))))
{
    BlockRegistry::Init(BLOCK_MANIFEST, m_BlockAtlasHandle, glm::vec2(16.0f, 16.0f));
    MusicManager::Init(MUSIC_DIR);

    m_ShaderLibrary.Load("terrain", "assets/shaders/terrain.glsl");
    m_ShaderLibrary.Load("entity", "assets/shaders/entity.glsl");
    
    m_BlockMaterial = CreateRef<Material>(m_ShaderLibrary.Get("terrain"));

    m_BlockMaterial->SetTexture(Scuffcraft::Get().GetAssetManager().GetAsset<Texture2D>(m_BlockAtlasHandle));

    m_CameraController.SetPosition({7.0f, 70.0f, 7.0f});
    m_CameraController.SetPitchYaw(-90.0f, 0.0f);
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
    // MusicManager::Start();

    auto steveTexture = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/textures/steve.png", AssetType::Texture2D);
    auto creeperTexture = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/textures/creeper.png", AssetType::Texture2D);
    auto witherTexture = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/textures/wither.png", AssetType::Texture2D);

    auto chestMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/chest.obj", AssetType::Mesh);
    auto steveMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/steve.obj", AssetType::Mesh);
    auto creeperMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/creeper.obj", AssetType::Mesh);
    auto witherMeshHandle = Scuffcraft::Get().GetAssetManager().LoadAsset("assets/models/wither.obj", AssetType::Mesh);

    auto steveMaterial = CreateRef<Material>(m_ShaderLibrary.Get("entity"));
    steveMaterial->SetTexture(Scuffcraft::Get().GetAssetManager().GetAsset<Texture2D>(steveTexture));

    auto creeperMaterial = CreateRef<Material>(m_ShaderLibrary.Get("entity"));
    creeperMaterial->SetTexture(Scuffcraft::Get().GetAssetManager().GetAsset<Texture2D>(creeperTexture));

    auto witherMaterial = CreateRef<Material>(m_ShaderLibrary.Get("entity"));
    witherMaterial->SetTexture(Scuffcraft::Get().GetAssetManager().GetAsset<Texture2D>(witherTexture));

    auto chest = m_World.CreateEntity();
    auto steve = m_World.CreateEntity();
    auto creeper = m_World.CreateEntity();
    auto wither = m_World.CreateEntity();

    chest.AddComponent<TransformComponent>(glm::vec3(0.0f, 65.0f, 0.0f));
    chest.AddComponent<StaticMeshComponent>(chestMeshHandle, m_BlockMaterial);

    steve.AddComponent<TransformComponent>(glm::vec3(0.0f, 65.0f, 0.0f));
    steve.AddComponent<StaticMeshComponent>(steveMeshHandle, steveMaterial);
    steve.AddComponent<CircularMotionComponent>(2.0f, 1.0f, 65.0f);

    creeper.AddComponent<TransformComponent>(glm::vec3(5.0f, 65.0f, 0.0f));
    creeper.AddComponent<StaticMeshComponent>(creeperMeshHandle, creeperMaterial);
    creeper.AddComponent<RotationComponent>(1.0f, 0.0f);

    wither.AddComponent<TransformComponent>(glm::vec3(-5.0f, 65.0f, 0.0f));
    wither.AddComponent<StaticMeshComponent>(witherMeshHandle, witherMaterial);
    wither.AddComponent<RotationComponent>(-1.0f, 0.0f);

    AudioEngine::PlayAt(Scuffcraft::Get().GetAssetManager().GetAsset<Audio>(m_AwMan), glm::vec3(5.0f, 65.0f, 0.0f));
}

void GameLayer::OnDetach()
{
    MusicManager::Stop();
}

void GameLayer::OnUpdateFixed(float dt)
{
    m_World.OnUpdate(dt, m_CameraController.GetCamera().GetPosition());
}

void GameLayer::OnUpdate(float dt)
{
    MusicManager::Update();

    m_CameraController.OnUpdate(dt);

    m_World.ProcessCompletedJobs();

    RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    RenderCommand::Clear();

    Renderer::BeginScene(m_CameraController.GetCamera());

    m_World.Draw(m_BlockMaterial);

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